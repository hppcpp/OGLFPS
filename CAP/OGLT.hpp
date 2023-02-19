/*
#include<GL/gl.h>

#include"CAP_GLT.hpp"
*/
namespace CAP
{
namespace OGL
{
class OglPPMatrix;

class OglPPMatrix
{
	private:
		OglPPMatrix(const OglPPMatrix&);
		OglPPMatrix& operator=(const OglPPMatrix&);
	public:
		OglPPMatrix(){glPushMatrix();}
		~OglPPMatrix(){glPopMatrix();}
};

template<typename TN>
inline void color3f(const TN* const);
template<typename TN>
inline void color4f(const TN* const);
template<typename TN>
inline void vertex2i(const TN* const);
template<typename TN>
inline void vertex3i(const TN* const);



//function
template<typename TN>
inline void color3f(const TN* const vec3){glColor3f(vec3[0],vec3[1],vec3[2]);}
template<typename TN>
inline void color4f(const TN* const vec4){glColor4f(vec4[0],vec4[1],vec4[2],vec4[3]);}
template<typename TN>
inline void vertex2i(const TN* const vec2){glVertex2i(vec2[0],vec2[1]);}
template<typename TN>
inline void vertex3i(const TN* const vec3){glVertex3i(vec3[0],vec3[1],vec3[2]);}

}

}
