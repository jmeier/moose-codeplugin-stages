# Stages MooseApp - Code-Plugin

This MooseApp code plugin contains a set of MooseObjects providing the [Stages] input file blocks.

**Overview** <a name="overview"></a>

In many Moose models, a large number of individual state changes must be modelled in a coordinated
and consistent manner. This functionality is required for modelling construction processes, for
example. While Moose does support such time-dependent model changes in principle, they must be
entered at a large number of points in the input file. As Moose input files can quickly have more
than 1000 lines, this results in a significant potential for errors and hinders quality control.

To ease the input and checking of model states, a group of Moose objects was created with the
'Stages' objects in this repository. This enables consecutive changes in the model to be defined
at a central location in a Moose input file.

**Table of Contents** <a name="toc"></a>
- [Setup for Users](#setup)
- [Usage](#usage)

## Setup for Users <a name="setup"></a>

If you want to use this code-plugin in your MooseApp, follow these steps:

- Clone this repository as git-submodule in a sub-folder in `./contrib/` of your MooseApp.
    Assuming you are in the root directory of your MooseApp this could be done with the following command:
  ```shell
  git submodule add https://github.com/jmeier/moose-codeplugin-stages.git contrib/stages
  ```
- In your MooseApp `Makefile`, insert the following line **directly above** the line `include $(FRAMEWORK_DIR)/app.mk` (if you have multiple code plugins, you need this line only once):
  ```MAKEFILE
  include $(wildcard $(CURDIR)/contrib/*/codeplugin.mk)
  ```

- Edit your MooseApp main .C-file (located in `./src/base/`; the file named `xyzApp.C` where `xyz` is the name of your app)
  - at the top where all the includes are, add:
    ```C++
    #include "StagesRegistrar.h"
    ```
  - insert the following line at the end of method `xyzApp::registerAll(Factory & f, ActionFactory & af, Syntax & s)`:
    ```C++
    StagesRegistrar::registerAll(s);
    ```

    The method should now look like this:

    ![install.png](img/install.png)


- Compile your MooseApp.

## Usage <a name="usage"></a>

This code plugin adds the `[Stages]` block to your MooseApp. With this block
changes may be staged over time in a moose simulation. The motivation for this
is to have a central location in your Moose input file to define the course of
events to be simulated (e.g. in terms of a construction process).

### `[Stages]` block and sub-blocks

The `[Stages]` block is to be added as a top-level block including all needed
sub-blocks. As a second level below the `[Stages]` block an arbitrary number
of `[Stage]` blocks my be added. Each of the `[Stage]` blocks has an associated
time given by the `t` parameter. It is not allowed to have two `[Stage]` blocks
with the same time. Inside each of the `[Stage]` blocks a number of sub-blocks
is to be added defining the staged changes.

```
[Stages]
  [Stage1]
    t = 1.0
    # sub-blocks defining the staged changes
  []
  [Stage2]
    t = 1.2
    # sub-blocks defining the staged changes
  []
  ⁞
[]
```

### `StagedTimeSequenceStepper` <a name="StagedTimeSequenceStepper"></a>

To ease synchronizing the staged changes in the `[Stages]` block and the
time steps of the model, a time stepper of type `StagedTimeSequenceStepper`
may be added in `[Executioner/TimeSteppers]`. This time stepper automatically
includes the times of all stages to the time stepping.

```
[Executioner]
  ⁞
  [TimeSteppers]
    [StagedTimeSequenceStepper1]
      type = StagedTimeSequenceStepper
    []
    ⁞
  []
  ⁞
[]
```

### Staged subdomain modification

Via the `StagedSubdomainModification` sub-block re-assignment of all elements
of a subdomain may be staged (providing the functionality of the
`TimedSubdomainModifier` available in Moose). Please note, that in addition to
the `StagedSubdomainModification` sub-blocks a `StagedSubdomainModifier` must
be given by the user for the re-assignment to be executed.

```
[MeshModifiers]
  [StagedSubdomainModifier]
    type = 'StagedSubdomainModifier'
    execute_on = 'INITIAL TIMESTEP_BEGIN'
  []
[]

[Stages]
  ⁞
  [StageX]
    t = 1.2
    [./StageX_Block2_To_Inactive]
      type = 'StagedSubdomainModification'
      from = 2
      to = ${Box2_inactive_id}
    []
  []
  ⁞
[]
```

### Staged function value change

Via the `StagedFunctionValueChange` sub-block function values may be staged.
Please note, that in addition to the `StagedFunctionValueChange` sub-blocks a `StagedFunction` must be given by the user and used where appropriate.

```
[Functions]
  [Box2_density]
    type = "StagedFunction"
  []
[]

[Stages]
  ⁞
  [Stage2]
    t = 1.2
    [./Stage1_Box2_Material]
      type = 'StagedFunctionValueChange'
      start_time = 't - 0.1'
      end_time = 't - 0.0001'
      function_names = 'Box2_density'
      new_values =     '${fparse 0.0001 * ${material_density} }'
    []
  []
  ⁞
[]
```

### Staged enabling and disabling of objects (Controls System)

Staged enabling and disabling of objects, by means of the Moose
`Controls System` is provided via the `StagedDisable` and `StagedEnable`
sub-blocks.
To ease reading of the input files, as alternative the `[Stage]`-block
offers the parameters `disable_objects` and `enable_objects` providing
the same functionality.

Example code for disabling objects:
```
[Stages]
  [Stage5]
    t = 5
    [./Stage5_Disable]
      type = 'StagedDisable'
      objects = 'Postprocessors::time_switch'
    []
  []
  [Stage9]
    t = 9
    disable_objects = 'Postprocessors::time_switch'
  []
[]
```

Example code for enabling objects:
```
[Stages]
  [Stage3]
    t = 3.0
    [./Stage3_Enable]
      type = 'StagedEnable'
      objects = 'Postprocessors::time_switch'
    []
  []
  [Stage7]
    t = 7
    enable_objects = 'Postprocessors::time_switch'
  []
[]
```

### Staged additional time steps

If additional time steps are required (e.g. between stages), the `StagedAdditionalTimeStep` block can be used in conjunction with a
[`StagedTimeSequenceStepper`](#StagedTimeSequenceStepper):

```
[Stages]
  [StageX]
    t = 3.0
    [StageX_AddtionalTimestep]
      type = 'StagedAdditionalTimeStep'
      time = 't - 0.2' #adds a time step shortly before 't'
    []
  []
[]
```
