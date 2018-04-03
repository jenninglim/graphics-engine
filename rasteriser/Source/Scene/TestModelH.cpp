#include "TestModelH.h"

void push_box(std::vector<Object *> &objects, std::vector<Triangle> &object_tri){
  Box *object = new Box(object_tri);
  objects.push_back(object);
  object_tri.clear();
}

void push_bunny(std::vector<Object *> &objects, std::vector<Triangle> &object_tri){
  Bunny *object = new Bunny(object_tri);
  objects.push_back(object);
  object_tri.clear();
}

void LoadCornellBox(Object * cornellBox){

}

void LoadObjects(std::vector<Object *>& objects )
{
  // Defines colors:
  vec3 red(    0.75f, 0.15f, 0.15f );
  vec3 yellow( 0.75f, 0.75f, 0.15f );
  vec3 green(  0.15f, 0.75f, 0.15f );
  vec3 cyan(   0.15f, 0.75f, 0.75f );
  vec3 blue(   0.15f, 0.15f, 0.75f );
  vec3 purple( 0.75f, 0.15f, 0.75f );
  vec3 white(  0.75f, 0.75f, 0.75f );

  vec4 A(L,0,0,1);
	vec4 B(0,0,0,1);
	vec4 C(L,0,L,1);
	vec4 D(0,0,L,1);

	vec4 E(L,L,0,1);
	vec4 F(0,L,0,1);
	vec4 G(L,L,L,1);
	vec4 H(0,L,L,1);

  std::vector<Triangle> triangles;
  triangles.clear();

  // Floor:
  triangles.push_back( Triangle( C, B, A, green ) );
  triangles.push_back( Triangle( C, D, B, green ) );

  // Left wall
  triangles.push_back( Triangle( A, E, C, purple ) );
  triangles.push_back( Triangle( C, E, G, purple ) );

  // Right wall
  triangles.push_back( Triangle( F, B, D, yellow ) );
  triangles.push_back( Triangle( H, F, D, yellow ) );

  // Ceiling
  triangles.push_back( Triangle( E, F, G, cyan ) );
  triangles.push_back( Triangle( F, H, G, cyan ) );

  // Back wall
  triangles.push_back( Triangle( G, D, C, white ) );
  triangles.push_back( Triangle( G, H, D, white ) );

  push_box(objects, triangles);
	// ---------------------------------------------------------------------------
	// Short block
	A = vec4(290,0,114,1);
	B = vec4(130,0, 65,1);
	C = vec4(240,0,272,1);
	D = vec4( 82,0,225,1);

	E = vec4(290,165,114,1);
	F = vec4(130,165, 65,1);
	G = vec4(240,165,272,1);
	H = vec4( 82,165,225,1);

  triangles.clear();

	// Front
	triangles.push_back( Triangle(E,B,A,red) );
	triangles.push_back( Triangle(E,F,B,red) );

	// Front
	triangles.push_back( Triangle(F,D,B,red) );
	triangles.push_back( Triangle(F,H,D,red) );

	// BACK
	triangles.push_back( Triangle(H,C,D,red) );
	triangles.push_back( Triangle(H,G,C,red) );

	// LEFT
	triangles.push_back( Triangle(G,E,C,red) );
	triangles.push_back( Triangle(E,A,C,red) );

	// TOP
	triangles.push_back( Triangle(G,F,E,red) );
	triangles.push_back( Triangle(G,H,F,red) );

  push_box(objects, triangles);

	// ---------------------------------------------------------------------------
	// Tall block

	A = vec4(423,0,247,1);
	B = vec4(265,0,296,1);
	C = vec4(472,0,406,1);
	D = vec4(314,0,456,1);

	E = vec4(423,330,247,1);
	F = vec4(265,330,296,1);
	G = vec4(472,330,406,1);
	H = vec4(314,330,456,1);

  triangles.clear();

	// Front
	triangles.push_back( Triangle(E,B,A,blue) );
	triangles.push_back( Triangle(E,F,B,blue) );

	// Front
	triangles.push_back( Triangle(F,D,B,blue) );
	triangles.push_back( Triangle(F,H,D,blue) );

	// BACK
	triangles.push_back( Triangle(H,C,D,blue) );
	triangles.push_back( Triangle(H,G,C,blue) );

	// LEFT
	triangles.push_back( Triangle(G,E,C,blue) );
	triangles.push_back( Triangle(E,A,C,blue) );

	// TOP
	triangles.push_back( Triangle(G,F,E,blue) );
	triangles.push_back( Triangle(G,H,F,blue) );

  push_box(objects, triangles);
  //Stanford Bunny
  /*
  vector<vec4> out_vertices;
  vector<vec3> out_faces;
  vector<Triangle> bunnytriangles;

  loadObj("./Source/bunny.obj",out_vertices,out_faces);
  cout << out_vertices.size() << endl;
  cout << out_faces.size() << endl;

  for(size_t i = 0; i<out_faces.size(); i++){
    vec3 face = out_faces[i];
    bunnytriangles.push_back( Triangle(
            1.0f*out_vertices[face.x-1],
            1.0f*out_vertices[face.y-1],
            1.0f*out_vertices[face.z-1], green));
  }

  push_bunny(objects, bunnytriangles);
  */
}
