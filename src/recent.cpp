#include <iostream>
#include <vector>

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <gtkmm.h>
#include <giomm/application.h>

enum class Command {
  ADD,
  REMOVE,
  LIST,
  PURGE,
  HELP
};

void run(int argc, char** argv)
{
  auto app = Gtk::Application::create("org.github.grumbel.recent",
                                      Gio::APPLICATION_HANDLES_COMMAND_LINE);

  Command command = Command::HELP;

  if (argc >= 2)
  {
    if (strcmp("add", argv[1]) == 0) {
      command = Command::ADD;
    } else if (strcmp("remove", argv[1]) == 0) {
      command = Command::REMOVE;
    } else if (strcmp("purge", argv[1]) == 0) {
      command = Command::PURGE;
    } else if (strcmp("list", argv[1]) == 0) {
      command = Command::LIST;
    } else if (strcmp("help", argv[1]) == 0) {
      command = Command::HELP;
    } else {
      throw std::runtime_error(fmt::format("unknown command: {}", argv[1]));
    }
  }

  std::vector<std::string> filenames;
  for (int i = 2; i < argc; ++i)
  {
    if (command == Command::ADD || command == Command::REMOVE)
    {
      filenames.emplace_back(argv[i]);
    }
    else
    {
      throw std::runtime_error(fmt::format("no file arguments allowed for '{}'", argv[1]));
    }
  }

  // Using create() instead of get_default() here, as otherwise it
  // won't flush properly and added items never show up
  auto recent_manager = Gtk::RecentManager::create();

  switch (command)
  {
    case Command::ADD:
      for (auto const& filename : filenames) {
        auto file = Gio::File::create_for_path(filename);
        bool const ret = recent_manager->add_item(file->get_uri());
        fmt::print("added {} {}\n", filename, file->get_uri(), ret);
      }
      break;

    case Command::REMOVE:
      for (auto const& filename : filenames) {
        auto file = Gio::File::create_for_path(filename);
        if (!recent_manager->remove_item(file->get_uri())) {
          fmt::print("removal of {} failed\n", filename);
        } else {
          fmt::print("removed {}\n", filename);
        }
      }
      break;

    case Command::LIST:
      for (auto const& item : recent_manager->get_items())
      {
        fmt::print("{}\n", item->get_uri().raw());
      }
      break;

    case Command::PURGE: {
      int const items_removed = recent_manager->purge_items();
      fmt::print("purged {} items\n", items_removed);
      break;
    }

    case Command::HELP:
      fmt::print("Usage: {} COMMAND [FILE]...\n"
                 "\n"
                 "Manipulate 'Recent Files' - ~/.local/share/recently-used.xbel\n"
                 "\n"
                 "Commands:\n"
                 "  add        Add files to Recent Files\n"
                 "  remove     Remove files from Recent Files\n"
                 "  list       Display recent files\n"
                 "  purge      Purge all entries from Recent Files\n"
                 "  help       Show this help\n"
                 , argv[0]);
      break;
  }

  app->run();
}


int main(int argc, char *argv[])
{
  try
  {
    run(argc, argv);
    return 0;
  }
  catch (Gtk::RecentManagerError const& err)
  {
    fmt::print(stderr, "error: {}\n", err.what().raw());
  }
  catch (std::exception const& err)
  {
    fmt::print(stderr, "error: {}\n", err.what());
  }
}

  /* EOF */
