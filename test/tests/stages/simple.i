# testing construction stages
# primary model units (m | s | kg --> N/m²)

#some model-specific variables
material_density = 21.5E3         # kg/m^3
gravitational_acceleration = 9.81 # m/s^2
sig_top = 0.0                     # N/m^2
z_top = 10.0

[Problem]
  kernel_coverage_check = false
  material_coverage_check = false
[]

Box2_inactive_id = '3'
inactive_block_ids = ${Box2_inactive_id}

[Mesh]

  [BaseMesh]
    type = GeneratedMeshGenerator
    elem_type = "TET10"
    dim = 3
    nx = 20
    ny = 20
    nz = 2
    xmin = -10
    xmax = +10
    ymin = -10
    ymax = +10
    zmin = -2
    zmax = +2
  []

  [Box1]
    type = SubdomainBoundingBoxGenerator
    input = "BaseMesh"
    block_id = 1
    location = "INSIDE"
    bottom_left = "-20 -20 -2"
    top_right   = "+20 +20 +2"
  []

  [Box2]
    type = SubdomainBoundingBoxGenerator
    input = "Box1"
    block_id = 2
    location = "INSIDE"
    bottom_left = "-2 -2 +2"
    top_right   = "+2 +2 0"
  []

  add_subdomain_ids = ${inactive_block_ids}

[]

active_block_ids =  '1 2' #Box1 Box2

[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
  large_kinematics = false
[]

# ===== Tensor mechanics for all active domain blocks =====
[Physics/SolidMechanics/QuasiStatic]
  [./all]
    new_system = true
    formulation = total
    strain = small

    block = ${active_block_ids}
    add_variables = true

    generate_output = 'stress_xx stress_xy stress_xz stress_yy stress_yz stress_zz '
  []
[]

# ===== Gravity =====
[Kernels]
  [./Gravity]
    type = Gravity
    block = ${active_block_ids}
    use_displaced_mesh = false
    variable = disp_z
    value = -${gravitational_acceleration}
  []
 [./donothing1]
    type = MatDiffusion
    block = ${inactive_block_ids}
    variable = disp_z
    diffusivity = 1e-7
  []
 [./donothing2]
    type = MatDiffusion
    block = ${inactive_block_ids}
    variable = disp_x
    diffusivity = 1e-7
  []
 [./donothing3]
    type = MatDiffusion
    block = ${inactive_block_ids}
    variable = disp_y
    diffusivity = 1e-7
  []
[]

# ===== Boundary Conditions: Fixies at XMin, XMax, YMin, YMax, ZMin =====
[BCs]

  [./BoundaryXMin_DispX]
    type = DirichletBC
    boundary = 'left'
    variable = disp_x
    value = 0
  []

  [./BoundaryXMax_DispX]
    type = DirichletBC
    boundary = 'right'
    variable = disp_x
    value = 0
  []

  [./BoundaryYMin_DispY]
    type = DirichletBC
    boundary = 'bottom'
    variable = disp_y
    value = 0
  []

  [./BoundaryYMax_DispY]
    type = DirichletBC
    boundary = 'top'
    variable = disp_y
    value = 0
  []

  [./BoundaryZMin_DispX]
    type = DirichletBC
    boundary = 'back'
    variable = disp_x
    value = 0
  []

  [./BoundaryZMin_DispY]
    type = DirichletBC
    boundary = 'back'
    variable = disp_y
    value = 0
  []

  [./BoundaryZMin_DispZ]
    type = DirichletBC
    boundary = 'back'
    variable = disp_z
    value = 0
  []

[]

# ===== Initial Conditions: Initial Stress Field =====
[Functions]
  [ini_xx]
    type = ParsedFunction
    expression = '(-sig_top - rho * g * (z_top - z)) * K0'
    symbol_names  = 'sig_top     z_top      rho                   g                               K0  '
    symbol_values = '${sig_top}  ${z_top}   ${material_density}   ${gravitational_acceleration}   0.3 '
  []
  [ini_yy]
    type = ParsedFunction
    expression = '-sig_top - rho * g * (z_top - z)'
    symbol_names  = 'sig_top     z_top      rho                   g                               K0  '
    symbol_values = '${sig_top}  ${z_top}   ${material_density}   ${gravitational_acceleration}   0.3 '
  []
  [ini_zz]
    type = ParsedFunction
    expression = '(-sig_top - rho * g * (z_top - z)) * K0'
    symbol_names  = 'sig_top     z_top      rho                   g                               K0  '
    symbol_values = '${sig_top}  ${z_top}   ${material_density}   ${gravitational_acceleration}   0.3 '
  []
  [Box2_elasticity_tensor_prefactor]
    type = "StagedFunction"
  []
  [Box2_density]
    type = "StagedFunction"
  []
[]


[Stages]

  [Stage0]
    t = 0.0
    [Stage0_Box2_Material]
      type = 'StagedFunctionValueChange'
      function_names = 'Box2_elasticity_tensor_prefactor  Box2_density'
      new_values =     '1.0                               ${fparse 1.0 * ${material_density} }'
    []
  []

  [Stage1]
    t = 0.4
    [./Stage1_Block2_To_Inactive]
      type = 'StagedSubdomainModification'
      from = 2
      to = ${Box2_inactive_id}
    []
    [./Stage1_Box2_Material]
      type = 'StagedFunctionValueChange'
      start_time = 't - 0.1'
      end_time = 't - 0.0001'
      function_names = 'Box2_elasticity_tensor_prefactor  Box2_density'
      new_values =     '0.0001                            ${fparse 0.0001 * ${material_density} }'
    []
  []

[]

# ===== Materials (linear-elastic to keep it simple) =====
[Materials]

  [./volAll_stress]
    type = ComputeLagrangianWrappedStress
    block = ${active_block_ids}
  []

  [./volAll_stress_base]
    type = ComputeLinearElasticStress
    block = ${active_block_ids}
  []

  [./volAll_ini_stress]
    type = ComputeEigenstrainFromInitialStress
    eigenstrain_name = ini_stress
    initial_stress = 'ini_xx 0 0  0 ini_yy 0  0 0 ini_zz'
    block = ${active_block_ids}
  []


  [./Box1_elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    block = 1
    youngs_modulus = 450E6 # 450 MN/m^2
    poissons_ratio = 0.15
  []

  [./Box1_density]
    type = GenericConstantMaterial  # switch to GenericFunctionMaterial, if staged!
    prop_names = density
    prop_values = ${material_density}
    block = 1
  []


  [./Box2_elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    block = 2
    youngs_modulus = 450E6 # 450 MN/m^2
    poissons_ratio = 0.15
    elasticity_tensor_prefactor = Box2_elasticity_tensor_prefactor
  []

  [./Box2_density]
    type = GenericFunctionMaterial
    prop_names = density
    prop_values = Box2_density
    constant_on = SUBDOMAIN
    block = 2
  []

[]

[MeshModifiers]
  [StagedSubdomainModifier]
    type = 'StagedSubdomainModifier'
    execute_on = 'INITIAL TIMESTEP_BEGIN'
  []
[]

# ===== Executioner =====
[Executioner]
  type = Transient

  end_time = 1.0
  [TimeSteppers]
    # [TimeSequenceStepper1]
    #   type = TimeSequenceStepper
    #   time_sequence = '0.5'
    # []
    # [TimeSequenceStepper2]
    #   type = TimeSequenceStepper
    #   time_sequence = '0.2 0.4 1.0'
    # []
    [StagedTimeSequenceStepper1]
      type = StagedTimeSequenceStepper
    []
  []

  solve_type = 'PJFNK'
  petsc_options = '-snes_converged_reason'
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  petsc_options_value = ' lu       mumps'

  nl_abs_tol = 1E-3
  nl_max_its = 400

  l_tol = 1E-3
  l_max_its = 200
[]

[Outputs]
  perf_graph = true
  exodus = true
[]

[Debug]
  # show_actions = true
  # show_action_dependencies = true
[]
