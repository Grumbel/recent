#include <iostream>
#include <vector>

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <gtkmm.h>
#include <giomm/application.h>

enum class Command {
  NONE,
  ADD,
  REMOVE,
  LIST,
  PURGE,
  HELP
};

Glib::ustring to_uri(std::string const& text)
{
  if (text.starts_with("file://")) {
    return text;
  } else {
    return Glib::filename_to_uri(Glib::canonicalize_filename(text));
  }
}

void run(int argc, char** argv)
{
  auto app = Gtk::Application::create("org.github.grumbel.recent",
                                      Gio::APPLICATION_HANDLES_COMMAND_LINE);

  Command command = Command::NONE;
  bool verbose = false;
  std::vector<std::string> filenames;

  for (int i = 1; i < argc; ++i)
  {
    if (argv[i][0] == '-') {
      // --option arguments
      if (strcmp("-v", argv[i]) == 0 ||
          strcmp("--verbose", argv[i]) == 0) {
        verbose = true;
      } else if (strcmp("-h", argv[i]) == 0 ||
                 strcmp("--help", argv[i]) == 0) {
        command == Command::HELP;
      } else {
        throw std::runtime_error(fmt::format("unrecognized option '{}'", argv[i]));
      }
    } else {
      // rest arguments
      if (command == Command::NONE) {
        if (strcmp("add", argv[i]) == 0) {
          command = Command::ADD;
        } else if (strcmp("remove", argv[i]) == 0) {
          command = Command::REMOVE;
        } else if (strcmp("purge", argv[i]) == 0) {
          command = Command::PURGE;
        } else if (strcmp("list", argv[i]) == 0) {
          command = Command::LIST;
        } else {
          throw std::runtime_error(fmt::format("unknown command '{}'", argv[i]));
        }
      } else {
        filenames.emplace_back(argv[i]);
      }
    }
  }

  // Using create() instead of get_default() here, as otherwise it
  // won't flush properly and added items never show up
  auto recent_manager = Gtk::RecentManager::create();

  switch (command)
  {
    case Command::ADD:
      for (auto const& filename : filenames) {
        Glib::ustring const& uri = to_uri(filename);
        recent_manager->add_item(uri);
        if (verbose) {
          fmt::print("added {}\n", uri.raw());
        }
      }
      break;

    case Command::REMOVE:
      for (auto const& filename : filenames) {
        Glib::ustring const& uri = to_uri(filename);
        recent_manager->remove_item(uri);
        if (verbose) {
          fmt::print("removed {}\n", uri.raw());
        }
      }
      break;

    case Command::LIST:
      for (auto const& item : recent_manager->get_items()) {
        fmt::print("{}\n", item->get_uri().raw());
      }
      break;

    case Command::PURGE: {
      int const items_removed = recent_manager->purge_items();
      if (verbose) {
        fmt::print("purged {} items\n", items_removed);
      }
      break;
    }

    case Command::NONE:
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
                 "\n"
                 "Options:\n"
                 "  -h, --help       Show this help\n"
                 "  -v, --verbose    Be more verbose\n"
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
  catch (Glib::Error const& err)
  {
    fmt::print(stderr, "error: {}\n", err.what().raw());
  }
  catch (std::exception const& err)
  {
    fmt::print(stderr, "error: {}\n", err.what());
  }
}

/* EOF */
