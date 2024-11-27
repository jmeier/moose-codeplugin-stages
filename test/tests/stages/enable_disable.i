# This tests controllability of the enable parameter of a MOOSE object via a
# set of times

[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 10
[]

# [Times]
#   [times_on]
#     type = InputTimes
#     # 8 should lie within the default window
#     times = '3 7'
#   []
#   [times_off]
#     type = InputTimes
#     # 8 should lie within the default window
#     times = '2 5 9'
#   []
#
#   [time_3]
#     type = InputTimes
#     times = '3'
#   []
#   [time_7]
#     type = InputTimes
#     times = '7'
#   []
#   [time_2]
#     type = InputTimes
#     times = '2'
#   []
#   [time_5]
#     type = InputTimes
#     times = '5'
#   []
#   [time_9]
#     type = InputTimes
#     times = '9'
#   []
# []

# [Controls]
#   [ctrl_times_on]
#     type = TimesEnableControl
#     times = times_on
#     reverse_on_false = false
#     enable_objects = 'Postprocessors::time_switch'
#     execute_on = 'INITIAL TIMESTEP_BEGIN'
#   []
#   [ctrl_times_off]
#     type = TimesEnableControl
#     times = times_off
#     reverse_on_false = false
#     disable_objects = 'Postprocessors::time_switch'
#     execute_on = 'INITIAL TIMESTEP_BEGIN'
#   []
# []

# [Controls]
#   [ctrl_time_3]
#     type = TimesEnableControl
#     times = time_3
#     reverse_on_false = false
#     enable_objects = 'Postprocessors::time_switch'
#     execute_on = 'INITIAL TIMESTEP_BEGIN'
#   []
#   [ctrl_time_7]
#     type = TimesEnableControl
#     times = time_7
#     reverse_on_false = false
#     enable_objects = 'Postprocessors::time_switch'
#     execute_on = 'INITIAL TIMESTEP_BEGIN'
#   []
#
#   [ctrl_time_2]
#     type = TimesEnableControl
#     times = time_2
#     reverse_on_false = false
#     disable_objects = 'Postprocessors::time_switch'
#     execute_on = 'INITIAL TIMESTEP_BEGIN'
#   []
#   [ctrl_time_5]
#     type = TimesEnableControl
#     times = time_5
#     reverse_on_false = false
#     disable_objects = 'Postprocessors::time_switch'
#     execute_on = 'INITIAL TIMESTEP_BEGIN'
#   []
#   [ctrl_time_9]
#     type = TimesEnableControl
#     times = time_9
#     reverse_on_false = false
#     disable_objects = 'Postprocessors::time_switch'
#     execute_on = 'INITIAL TIMESTEP_BEGIN'
#   []
# []


#[Controls]
#  [ctrl_time_3]
#    type = TimedEnableControl
#    time = 3
#    enable_objects = 'Postprocessors::time_switch'
#    execute_on = 'INITIAL TIMESTEP_BEGIN'
#  []
#  [ctrl_time_7]
#    type = TimedEnableControl
#    time = 7
#    enable_objects = 'Postprocessors::time_switch'
#    execute_on = 'INITIAL TIMESTEP_BEGIN'
#  []#
#  [ctrl_time_2]
#    type = TimedEnableControl
#    time = 2
#    disable_objects = 'Postprocessors::time_switch'
#    execute_on = 'INITIAL TIMESTEP_BEGIN'
#  []
#  [ctrl_time_5]
#    type = TimedEnableControl
#    time = 5
#    disable_objects = 'Postprocessors::time_switch'
#    execute_on = 'INITIAL TIMESTEP_BEGIN'
#  []
#  [ctrl_time_9]
#    type = TimedEnableControl
#    time = 9
#    disable_objects = 'Postprocessors::time_switch'
#    execute_on = 'INITIAL TIMESTEP_BEGIN'
#  []
#[]



[Stages]
  [Stage0]
    t = 0.0
  []
  [Stage1]
    t = 1
  []
  [Stage2]
    t = 2
    [./Stage2_Disable]
      type = 'StagedDisable'
      objects = 'Postprocessors::time_switch'
    []
  []
  [Stage3]
    t = 3.0
    [./Stage3_Enable]
      type = 'StagedEnable'
      objects = 'Postprocessors::time_switch'
    []
  []
  [Stage4]
    t = 4
  []
  [Stage5]
    t = 5
    [./Stage5_Disable]
      type = 'StagedDisable'
      objects = 'Postprocessors::time_switch'
    []
  []
  [Stage6]
    t = 6
  []
  [Stage7]
    t = 7
    enable_objects = 'Postprocessors::time_switch'
  []
  [Stage8]
    t = 8
  []
  [Stage9]
    t = 9
    disable_objects = 'Postprocessors::time_switch'
  []
  [Stage10]
    t = 10
  []
  [Stage11]
    t = 11
  []
  [Stage12]
    t = 12
  []
[]

[Debug]
  show_execution_order = ALWAYS
[]

[Postprocessors]
  [time_ref]
    type = TimePostprocessor
    execute_on = 'INITIAL TIMESTEP_BEGIN'
  []
  [time_switch]
    type = TimePostprocessor
    execute_on = 'INITIAL TIMESTEP_BEGIN'
  []
[]

[Problem]
  solve = false
[]

[Executioner]
  type = Transient

  dt = 1
  num_steps = 10

  #[TimeSteppers]
  #  [StagedTimeSequenceStepper1]
  #    type = StagedTimeSequenceStepper
  #  []
  #[]
[]

[Outputs]
  csv = true
[]
