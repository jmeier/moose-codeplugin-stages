# This tests the StagedFunction

[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 10
[]

# ===== Functions to be used by the Stages-Blocks =====
[Functions]
  [MyStagedFunctions]
    type = "StagedFunction"
  []
[]

[Stages]
  [Stage0]
    t = 0.0
    [Stage0_MyStagedFunctions]
      type = 'StagedFunctionValueChange'
      function_names = 'MyStagedFunctions'
      new_values = '    1.0'
    []
  []
  [Stage1]
    t = 1
    [Stage1_MyStagedFunctions]
      type = 'StagedFunctionValueChange'
      function_names = 'MyStagedFunctions'
      new_values = '    2.0'
      step_function_type = 'LINEAR'
    []
  []
  [Stage2]
    t = 2
    [Stage2_MyStagedFunctions]
      type = 'StagedFunctionValueChange'
      function_names = 'MyStagedFunctions'
      new_values = '    3.0'
      start_time = 't-0.5'
      step_function_type = 'LINEAR'
    []
  []
  [Stage3]
    t = 3.0
    [Stage3_MyStagedFunctions]
      type = 'StagedFunctionValueChange'
      function_names = 'MyStagedFunctions'
      new_values = '    4.0'
    []
  []
  [Stage4]
    t = 4
  []
  [Stage5]
    t = 5
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
  [function_value]
    type = FunctionValuePostprocessor
    execute_on = 'INITIAL TIMESTEP_BEGIN'
    function = 'MyStagedFunctions'
  []
[]

[Problem]
  solve = false
[]

[Executioner]
  type = Transient

  dt = .1
  num_steps = 50
[]

[Outputs]
  csv = true
[]
