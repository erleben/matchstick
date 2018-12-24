#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <geometry_convert.h>
#include <geometry_inside.h>
#include <geometry_is_valid.h>
#include <geometry_make_intersection.h>
#include <geometry_make_union.h>
#include <geometry_transform.h>

#include <barycentric/geometry_barycentric.h>
#include <barycentric/geometry_barycentric_inside.h>
#include <barycentric/geometry_barycentric_derivatives.h>

#include <types/geometry_direction_table.h>
#include <types/geometry_interval.h>
#include <types/geometry_dop.h>
#include <types/geometry_line.h>
#include <types/geometry_plane.h>
#include <types/geometry_ray.h>
#include <types/geometry_aabb.h>
#include <types/geometry_capsule.h>
#include <types/geometry_cylinder.h>
#include <types/geometry_box.h>
#include <types/geometry_obb.h>
#include <types/geometry_sphere.h>
#include <types/geometry_tetrahedron.h>
#include <types/geometry_triangle.h>
#include <types/geometry_gauss_map_of_convex_polyhedra.h>
#include <types/geometry_point.h>

#include <overlap/geometry_overlap_dop_dop.h>
#include <overlap/geometry_overlap_interval_interval.h>
#include <overlap/geometry_overlap_obb_obb.h>
#include <overlap/geometry_overlap_tetrahedron_tetrahedron.h>
#include <overlap/geometry_overlap_triangle_triangle.h>
#include <overlap/geometry_overlap_obb_capsule.h>

#include <raycast/geometry_raycast_aabb.h>
#include <raycast/geometry_raycast_plane.h>
#include <raycast/geometry_raycast_dop.h>
#include <raycast/geometry_raycast_triangle.h>
#include <raycast/geometry_raycast_tetrahedron.h>
#include <raycast/geometry_raycast_sphere.h>
#include <raycast/geometry_raycast_obb.h>

#include <closest_points/geometry_closest_point_on_line.h>
#include <closest_points/geometry_closest_point_on_plane.h>
#include <closest_points/geometry_closest_points_line_line.h>
#include <closest_points/geometry_closest_points_tetrahedron_tetrahedron.h>

#include <contacts/geometry_contacts_callback.h>
#include <contacts/geometry_contacts_tetrahedron_tetrahedron_opposing.h>
#include <contacts/geometry_contacts_tetrahedron_tetrahedron_growth.h>
#include <contacts/geometry_contacts_tetrahedron_tetrahedron_closest_points.h>

//GEOMETRY_H
#endif
