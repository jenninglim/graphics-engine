class Triangle
{
public:
	glm::vec4 v0;
	glm::vec4 v1;
	glm::vec4 v2;
	glm::vec4 normal;
	glm::vec3 color;

	Triangle( glm::vec4 v0, glm::vec4 v1, glm::vec4 v2, glm::vec3 color )
		: v0(v0), v1(v1), v2(v2), color(color)
	{
		ComputeNormal();
	}

	void ComputeNormal()
	{
	  glm::vec3 e1 = glm::vec3(v1.x-v0.x,v1.y-v0.y,v1.z-v0.z);
	  glm::vec3 e2 = glm::vec3(v2.x-v0.x,v2.y-v0.y,v2.z-v0.z);
	  glm::vec3 normal3 = glm::normalize( glm::cross( e2, e1 ) );
	  normal.x = normal3.x;
	  normal.y = normal3.y;
	  normal.z = normal3.z;
	  normal.w = 1.0;
	}

	void RotateXAxis(float radians){
		glm::vec4 s0(0, glm::cos(radians), -glm::sin(radians),0);
		glm::vec4 s1(0, glm::sin(radians), glm::cos(radians),0);
		glm::vec4 s2(1, 0, 0,0);
		glm::mat4 R = glm::mat4(s2,s0,s1,glm::vec4(0,0,0,1));
		v0 = R*v0;
		v1 = R*v1;
		v2 = R*v2;
		ComputeNormal();
	}

	void Translate(glm::vec4 translation){
		v0 = v0 + translation;
		v1 = v1 + translation;
		v2 = v2 + translation;
	}

	glm::vec4 getAveragePosition(){
		glm::vec4 ap = (v0 + v1 + v2) / 3.0f;
		return ap;
	}
};
