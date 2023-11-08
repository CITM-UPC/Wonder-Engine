#include "Primitive.h"
#include "Globals.h"

Cube::Cube(): 
	a(-1, -1, 1),
	b(1, -1, 1),
	c(1, 1, 1),
	d(-1, 1, 1),
	e(-1, -1, -1),
	f(1, -1, -1),
	g(1, 1, -1),
	h(-1, 1, -1) 
{}

Sphere::Sphere(){

    for (int i = 0; i <= num_subdivisions; ++i) {
        double lat0 = PI * (-0.5f + (i - 1) / num_subdivisions);
        double z0 = radius * sin(lat0);
        double zr0 = radius * cos(lat0);

        double lat1 = PI * (-0.5f + i / num_subdivisions);
        double z1 = radius * sin(lat1);
        double zr1 = radius * cos(lat1);

        for (int j = 0; j <= num_subdivisions; ++j) {
            double lng = 2 * PI * (j - 1) / num_subdivisions;
            double x = cos(lng);
            double y = sin(lng);

            vertices.push_back(vec3f(x * zr0, y * zr0, z0));
            vertices.push_back(vec3f(x * zr1, y * zr1, z1));
        }
    }
}