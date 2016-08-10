import mcellSwig as m

world = m.mcell_create()

m.mcell_init_state(world)

iterations = 100
m.mcell_set_time_step(world,1e-6)
m.mcell_set_iterations(world,iterations)

species_def = m.mcell_species_spec()
species_def.name = "x"
species_def.D = 1e-6
species_def.is_2d = 0
species_def.custom_time_step = 0
species_def.target_only = 0
species_def.max_step_length = 0

sym = m.mcell_symbol();
mcell_sym = m.mcell_create_species(world, species_def, sym)
m.mcell_print_name(mcell_sym)

scene = m.object()
real_scene = m.mcell_create_instance_object(world, "Scene", scene)

position = m.vector3()
position.x = 0.0
position.y = 0.0
position.z = 0.0
print("position")
print position
diameter = m.vector3()
diameter.x = .00999
diameter.y = .00999
diameter.z = .00999

x_releaser = m.object()
print ("x_releaser")
print x_releaser

x_foo= m.mcell_add_to_species_list(  mcell_sym  , False, 0, None);
print("x_foo")
print(x_foo)

rel_object = m.object()
release_object = m.mcell_create_geometrical_release_site(world , real_scene, "B_releaser", m.SHAPE_SPHERICAL, position, diameter, x_foo, 5000, 1, None, rel_object)

viz_list = m.mcell_add_to_species_list(mcell_sym, False, 0, None);
m.mcell_create_viz_output(world, "./viz_data/test", viz_list, 0, iterations, 1)

print (x_foo)
print ("deleting x_foo")
m.mcell_delete_species_list(x_foo)
print("printing whats left of x_foo")
print(x_foo)


print("x_releaser")
print (release_object)

report_flags = m.REPORT_WORLD;


c_list = m.output_column_list()
# -100 is in the place of OREINT_NOT_SET (used typemap for mcell_create_count in mcell_react_out.i) because limits.h stuff does not work well with swig.......
count_list = m.mcell_create_count(world, mcell_sym, -100, None, report_flags, None, c_list)
print("count list")
print(count_list)

"""
os = m.output_set()
os = m.mcell_create_new_output_set(None, 0, count_list.column_head, m.FILE_SUBSTITUTE, "react_data/foobar.dat")

outTimes = m.output_times_inlist()
outTimes.type = OUTPUT_BY_STEP
outTimes.step = 1e-5

output = m.output_set_list()
output.set_head = os
output.set_tail = os

m.mcell_add_reaction_output_block(world, output, 10000, outTimes)

"""

#mcell_create_geometrical_release_site(state, world_object, "B_releaser", SHAPE_SPHERICAL, &position, &diameter, B, 5000, 1, NULL, &B_releaser), "could not create B_releaser");

#mcell_delete_species_list(B);



m.mcell_init_simulation(world)
m.mcell_init_output(world)
m.mcell_run_simulation(world)