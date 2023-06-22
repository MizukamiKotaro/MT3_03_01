#include "Collision.h"
#include"calc.h"
#include"MyVector3.h"
#include<cmath>

bool Collision::IsCollision(const Sphere& s1, const Sphere& s2) {
	float length = Calc::MakeLength(s1.center_, s2.center_);

	//スケールはいじるな！お願いします。
	if (length <= s1.radius_ + s2.radius_) {
		return true;
	}
	return false;
}

bool Collision::IsCollision(const Sphere& sphere, const Plane& plane) {
	float length = Calc::MakeLength(sphere, plane);

	if (length <= sphere.radius_) {
		if (plane.isLimit) {
			MyVector3 center = plane.normal;
			center *= plane.distance;

			MyVector3 perpendiculars[4];

			perpendiculars[0] = Calc::Normalize(Calc::Perpendicular(plane.normal));
			perpendiculars[1] = { -perpendiculars[0].x,-perpendiculars[0].y, -perpendiculars[0].z };
			perpendiculars[2] = Calc::Cross(plane.normal, perpendiculars[0]);
			perpendiculars[3] = { -perpendiculars[2].x,-perpendiculars[2].y,-perpendiculars[2].z };

			for (int32_t index = 0; index < 4; index++) {
				perpendiculars[index] = center + perpendiculars[index] * 2.0f;
			}

			MyVector3 pos = Calc::Project(center - sphere.center_, plane.normal) + sphere.center_;
			float radius = sqrtf(powf(sphere.radius_, 2.0f) - powf(length, 2.0f));

			float len;
			len = Calc::MakeLength(pos, perpendiculars[0], perpendiculars[2]);
			if (len <= radius) {
				return true;
			}
			len = Calc::MakeLength(pos, perpendiculars[0], perpendiculars[3]);
			if (len <= radius) {
				return true;
			}
			len = Calc::MakeLength(pos, perpendiculars[1], perpendiculars[2]);
			if (len <= radius) {
				return true;
			}
			len = Calc::MakeLength(pos, perpendiculars[1], perpendiculars[2]);
			if (len <= radius) {
				return true;
			}

			float dot[4];

			dot[0] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[2], perpendiculars[0] - pos));
			dot[1] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[1], perpendiculars[2] - pos));
			dot[2] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[3], perpendiculars[1] - pos));
			dot[3] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[0], perpendiculars[3] - pos));

			if ((dot[0] > 0 && dot[1] > 0 && dot[2] > 0 && dot[3] > 0) || (dot[0] < 0 && dot[1] < 0 && dot[2] < 0 && dot[3] < 0)) {
				return true;
			}
		}
		else {
			return true;
		}
	}
	return false;
}

bool Collision::IsCollision(const Segment& segment, const Plane& plane) {

	if (Calc::Dot(segment.diff, plane.normal) == 0) {

		MyVector3 pos = plane.normal;
		pos *= plane.distance;

		Line tmpLine = { segment.origin,plane.normal };

		pos = Calc::ClosestPoint(pos, tmpLine);

		if (Calc::MakeLength(pos, segment.origin) == 0) {

			return true;
		}
		return false;
	}

	float t = (plane.distance - Calc::Dot(segment.origin, plane.normal)) / Calc::Dot(segment.diff, plane.normal);

	if (t >= 0 && t <= 1) {
		if (plane.isLimit) {
			MyVector3 pos = plane.normal;
			pos *= plane.distance;

			MyVector3 perpendiculars[4];

			perpendiculars[0] = Calc::Normalize(Calc::Perpendicular(plane.normal));
			perpendiculars[1] = { -perpendiculars[0].x,-perpendiculars[0].y, -perpendiculars[0].z };
			perpendiculars[2] = Calc::Cross(plane.normal, perpendiculars[0]);
			perpendiculars[3] = { -perpendiculars[2].x,-perpendiculars[2].y,-perpendiculars[2].z };

			for (int32_t index = 0; index < 4; index++) {
				perpendiculars[index] = pos + perpendiculars[index] * 2.0f;
			}

			pos = MyVector3(segment.origin) + MyVector3(segment.diff) * t;

			float dot[4];

			dot[0] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[2], perpendiculars[0] - pos));
			dot[1] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[1], perpendiculars[2] - pos));
			dot[2] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[3], perpendiculars[1] - pos));
			dot[3] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[0], perpendiculars[3] - pos));

			if ((dot[0] > 0 && dot[1] > 0 && dot[2] > 0 && dot[3] > 0) || (dot[0] < 0 && dot[1] < 0 && dot[2] < 0 && dot[3] < 0)) {
				return true;
			}

			return false;
		}

		return true;
	}
	return false;

}
bool Collision::IsCollision(const Ray& ray, const Plane& plane) {
	if (Calc::Dot(ray.diff, plane.normal) == 0) {
		MyVector3 pos = plane.normal;
		pos *= plane.distance;

		Line tmpLine = { ray.origin,plane.normal };

		pos = Calc::ClosestPoint(pos, tmpLine);

		if (Calc::MakeLength(pos, ray.origin) == 0) {

			return true;
		}
		return false;
	}

	float t = (plane.distance - Calc::Dot(ray.origin, plane.normal)) / Calc::Dot(ray.diff, plane.normal);

	if (t >= 0) {
		if (plane.isLimit) {
			MyVector3 pos = plane.normal;
			pos *= plane.distance;

			MyVector3 perpendiculars[4];

			perpendiculars[0] = Calc::Normalize(Calc::Perpendicular(plane.normal));
			perpendiculars[1] = { -perpendiculars[0].x,-perpendiculars[0].y, -perpendiculars[0].z };
			perpendiculars[2] = Calc::Cross(plane.normal, perpendiculars[0]);
			perpendiculars[3] = { -perpendiculars[2].x,-perpendiculars[2].y,-perpendiculars[2].z };

			for (int32_t index = 0; index < 4; index++) {
				perpendiculars[index] = pos + perpendiculars[index] * 2.0f;
			}

			pos = MyVector3(ray.origin) + MyVector3(ray.diff) * t;

			float dot[4];

			dot[0] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[2], perpendiculars[0] - pos));
			dot[1] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[1], perpendiculars[2] - pos));
			dot[2] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[3], perpendiculars[1] - pos));
			dot[3] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[0], perpendiculars[3] - pos));

			if ((dot[0] > 0 && dot[1] > 0 && dot[2] > 0 && dot[3] > 0) || (dot[0] < 0 && dot[1] < 0 && dot[2] < 0 && dot[3] < 0)) {
				return true;
			}

			return false;
		}

		return true;
	}
	return false;

}
bool Collision::IsCollision(const Line& line, const Plane& plane) {

	if (Calc::Dot(line.diff, plane.normal) == 0) {
		MyVector3 pos = plane.normal;
		pos *= plane.distance;

		Line tmpLine = { line.origin,plane.normal };

		pos = Calc::ClosestPoint(pos, tmpLine);

		if (Calc::MakeLength(pos, line.origin) == 0) {

			return true;
		}
		return false;
	}

	if (plane.isLimit) {
		float t = (plane.distance - Calc::Dot(line.origin, plane.normal)) / Calc::Dot(line.diff, plane.normal);

		MyVector3 pos = plane.normal;
		pos *= plane.distance;

		MyVector3 perpendiculars[4];

		perpendiculars[0] = Calc::Normalize(Calc::Perpendicular(plane.normal));
		perpendiculars[1] = { -perpendiculars[0].x,-perpendiculars[0].y, -perpendiculars[0].z };
		perpendiculars[2] = Calc::Cross(plane.normal, perpendiculars[0]);
		perpendiculars[3] = { -perpendiculars[2].x,-perpendiculars[2].y,-perpendiculars[2].z };

		for (int32_t index = 0; index < 4; index++) {
			perpendiculars[index] = pos + perpendiculars[index] * 2.0f;
		}

		pos = MyVector3(line.origin) + MyVector3(line.diff) * t;

		float dot[4];

		dot[0] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[2], perpendiculars[0] - pos));
		dot[1] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[1], perpendiculars[2] - pos));
		dot[2] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[3], perpendiculars[1] - pos));
		dot[3] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[0], perpendiculars[3] - pos));

		if ((dot[0] > 0 && dot[1] > 0 && dot[2] > 0 && dot[3] > 0) || (dot[0] < 0 && dot[1] < 0 && dot[2] < 0 && dot[3] < 0)) {
			return true;
		}

		return false;
	}

	return true;
}

