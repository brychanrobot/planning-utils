
libs= ['m', 'GL', 'GLU', 'glfw', 'glut',
'X11', 'Xrandr', 'Xinerama', 'Xi', 'Xxf86vm', 'Xcursor',
'pthread', 'dl', 'boost_system']


env=Environment(CXXFLAGS='-Wall -O3 -march=native -std=c++14', LIBS=libs)
debug = ARGUMENTS.get('debug', 0)
if int(debug):
	env.Append(CCFLAGS = '-g')

env.VariantDir('build', '.', duplicate=0)

objects = [
	env.Object('build/geom/Rect.cpp'),
]

env.Program(['build/example.cpp'] + objects)
