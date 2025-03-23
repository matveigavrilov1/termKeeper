# TermKeeper

TermKeeper is a software application designed for storing and managing frequently used terminal commands. Upon launch, the program resides in the Windows system tray and awaits the activation of a hotkey (default: Ctrl+Alt+R). When the hotkey is pressed, a window opens, allowing users to create, edit, delete, and select commands (by pressing Enter). The commands are organized in a hierarchical structure similar to a file system, where users can create and delete folders to better organize their commands. 

Selected commands are cached for quick access, and both the commands and the command cache are stored in XML format (storage.xml and cache.xml, respectively). The application is configured using a settings.yaml file, which allows users to customize various aspects of the program, including hotkey assignments and storage paths. Once a command is selected and Enter is pressed, the window closes, and the command is copied to the clipboard. Cached commands can be accessed and managed through the dedicated Cache window.

# Build with Docker

```
docker build -t termkeeperimage .
docker run --rm -v $(pwd):/termKeeper termkeeperimage
```

# Configure

| Parameter                     | Description                                                                          | Default          | Type       |
|-------------------------------|--------------------------------------------------------------------------------------|------------------|------------|
| **tkHotKeyBinder**            | Settings for a program that resides in the Windows tray and waits for a hotkey press |                  | `object`   |
| &ensp;executable              | Name of the file to be executed on hotkey press                                      | `Executable.exe` | `string`   |
| &ensp;hotKey                  | Hotkey to trigger the executable file                                                | `Ctrl+Alt+R`     | `string`   |
| **tkExecutable**              | Settings for the CLI displayed on hotkey press                                       |                  | `object`   |
| &ensp;closeOnChoice           | If `true`, the window closes after a command is selected                             | `true`           | `boolean`  |
| &ensp;windows                 | Window display settings in the CLI                                                   |                  | `object`   |
| &ensp;&ensp;registrated       | List of available windows (window sizes are fixed)                                   | `Menu`, `Storage`, `Cache` | `array`    |
| &ensp;&ensp;activated         | List of windows displayed on startup (order matters)                                 | `Menu`, `Storage` | `array`    |
| &ensp;&ensp;menu              | List of windows displayed in the menu (order matters)                                | `Storage`, `Cache` | `array`    |
| &ensp;&ensp;initialController | Window that handles input on startup                                                 | `Menu`           | `string`   |
| &ensp;storage                 | Command storage settings                                                             |                  | `object`   |
| &ensp;&ensp;storageFile       | File for storing data                                                                | `storage.xml`    | `string`   |
| &ensp;cache                   | Cache settings                                                                       |                  | `object`   |
| &ensp;&ensp;cacheFile         | File for caching data                                                                | `cache.xml`      | `string`   |
| &ensp;&ensp;cacheMaxSize      | Maximum cache size                                                                   | `10`             | `integer`  |
| &ensp;screenSize              | Screen size (both width and height must be specified)                                |                  | `object`   |
| &ensp;&ensp;width             | Screen width (0 — automatic adjustment)                                              | `0`              | `integer`  |
| &ensp;&ensp;height            | Screen height (0 — automatic adjustment)                                             | `0`              | `integer`  |
| &ensp;hintsPresets            | Presets for user hints. Hint format: `<Key>:<Description>`                           |                  | `object`   |
| &ensp;&ensp;storageSelectionMode | Hints displayed in selection mode in the storage window                           |                  | `object`   |
| &ensp;&ensp;storageInputMode  | Hints displayed in input mode in the storage window                                  |                  | `object`   |
| &ensp;&ensp;cache             | Hints displayed in the cache window                                                  |                  | `object`   |
