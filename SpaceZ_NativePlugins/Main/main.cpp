#include<iostream>
#include"Core/CoreAdd.h"
#include"Core/Tools/Vector3.h"
#include"Core/SpaceGeomBody/Bound.h"
using namespace Core;
using namespace Core::SpaceGeomBody;

int main()
{
    std::cout << ToString(Bound(Vector3::zero,Vector3::one)) << std::endl;
    return 0;
}
