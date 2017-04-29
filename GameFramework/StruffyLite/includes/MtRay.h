////////////////////////////////////////////////////////////////////////////////
// MtRay.h

// Include guard
#ifndef __MtRay_h__
#define __MtRay_h__

// Forward declarations
struct MtVector3;
struct MtVector2;
class MtTriangle;
class MtPlane;

// Class definition
class MtRay
{
 public:

	// Constructors
    MtRay()
    {
       point1 = point2 = MtVector3(0, 0, 0);
    }

    MtRay(MtVector3 &a, MtVector3 &b)
    {
       point1 = a;
       point2 = b;
    }

    // Public functions
    bool Collide(const MtTriangle &triangle, MtVector3 &intersection);
    bool CollideWithPlane(const MtPlane &plane, MtVector3 &intersection);

    // Member variables
    MtVector3		point1;
    MtVector3		point2;
};

#endif // __MtRay_h__
