-- A simple scene with some miscellaneous geometry.

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25)
mat4 = gr.material({0.9, 0.6, 0.0}, {0.5, 0.4, 0.8}, 25)

scene_root = gr.node('root')


b4 = gr.nh_box('b4', {0, 105, 0}, 100)
scene_root:add_child(b4)
b4:set_material(mat4)

b1 = gr.nh_box('b1', {0, 0, 0}, 100)
scene_root:add_child(b1)
b1:set_material(mat4)

b2 = gr.nh_box('b2', {0, -105, 0}, 100)
scene_root:add_child(b2)
b2:set_material(mat4)

b3 = gr.nh_box('b3', {0, -210, 0}, 100)
scene_root:add_child(b3)
b3:set_material(mat4)

b5 = gr.nh_box('b5', {-105, 0, 0}, 100)
scene_root:add_child(b5)
b5:set_material(mat4)

b6 = gr.nh_box('b6', {105, 0, 0}, 100)
scene_root:add_child(b6)
b6:set_material(mat4)

b7 = gr.nh_box('b7', {0, 0, -105}, 100)
scene_root:add_child(b7)
b7:set_material(mat4)

b8 = gr.nh_box('b8', {0, 0, 105}, 100)
scene_root:add_child(b8)
b8:set_material(mat4)

s2 = gr.nh_sphere('s2', {200, 50, -500}, 150)
scene_root:add_child(s2)
s2:set_material(mat1)

s3 = gr.nh_sphere('s3', {0, -1500, -500}, 1000)
scene_root:add_child(s3)
s3:set_material(mat2)

white_light = gr.light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
orange_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'sample.png', 1440, 900,
	  {400, 0, 800}, {-0.3, 0, -0.7}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light, orange_light})
