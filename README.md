# Stages mooseapp-code-plugin

This MooseApp code plugin contains a set of MooseObjects providing the [Stages] input file blocks.


## Setup for Users

If you want to use this code-plugin in your MooseApp, follow these steps:

- Clone this repository as git-submodule in a sub-folder in `./contrib/` of your MooseApp

- In your MooseApp `Makefile`, insert the following line **directly above** the line `include $(FRAMEWORK_DIR)/app.mk` (if you have multiple code plugins, you need this line only once):
  ```
  include $(wildcard $(CURDIR)/contrib/*/codeplugin.mk)
  ```

- Compile your MooseApp.
