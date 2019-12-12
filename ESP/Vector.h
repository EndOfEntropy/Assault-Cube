#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>

using namespace std;

class Vec3
{
public:
	float m_xPos{};
	float m_yPos{};
	float m_zPos{};


public:
	Vec3(float xPos, float yPos, float zPos) :
		m_xPos(xPos), m_yPos(yPos), m_zPos(zPos)
	{
	}

	Vec3()
	{
	}

	~Vec3()
	{
	}

	float getxPos() { return m_xPos; }
	float getyPos() { return m_yPos; }
	float getzPos() { return m_zPos; }

	void PrintVector()
	{
		cout << m_xPos << ' ' << m_yPos << ' ' << m_zPos << "\n";
	};

	Vec3 AddVector(Vec3 &v)
	{
		Vec3 vector;
		vector.m_xPos = m_xPos + v.m_xPos;
		vector.m_yPos = m_yPos + v.m_yPos;
		vector.m_zPos = m_zPos + v.m_zPos;

		return vector;
	}

	Vec3 SubVector(Vec3 &v)
	{
		Vec3 vector;
		vector.m_xPos = m_xPos - v.m_xPos;
		vector.m_yPos = m_yPos - v.m_yPos;
		vector.m_zPos = m_zPos - v.m_zPos;

		return vector;
	}

	Vec3 MultiVector(float f)
	{
		Vec3 vector;
		vector.m_xPos = m_xPos * f;
		vector.m_yPos = m_yPos * f;
		vector.m_zPos = m_zPos * f;

		return vector;
	}

	float GetVectorLength()
	{
		return sqrt((m_xPos * m_xPos) + (m_yPos * m_yPos) + (m_zPos * m_zPos));
	}

	Vec3 NormalizeVector()
	{
		Vec3 vector;
		float length = vector.GetVectorLength();

		if (length != 0) {
			vector.m_xPos = m_xPos / length;
			vector.m_yPos = m_yPos / length;
			vector.m_zPos = m_zPos / length;
		}

		return vector;
	}

	float GetDistanceTo(Vec3 &v)

	{
		float distance = sqrt(((v.m_xPos - m_xPos) * (v.m_xPos - m_xPos)) + ((v.m_yPos - m_yPos) * (v.m_yPos - m_yPos)) + ((v.m_zPos - m_zPos) * (v.m_zPos - m_zPos)));
		return distance;
	}

};


class Vec2
{
public:
	float m_xPos{};
	float m_yPos{};


public:
	Vec2(float xPos, float yPos) :
		m_xPos(xPos), m_yPos(yPos)
	{
	}

	Vec2()
	{
	}

	~Vec2()
	{
	}

	float getxPos() { return m_xPos; }
	float getyPos() { return m_yPos; }

	void PrintVector()
	{
		cout << m_xPos << ' ' << m_yPos << ' ' << "\n";
	};

	Vec2 AddVector(Vec2 &v)
	{
		Vec2 vector;
		vector.m_xPos = m_xPos + v.m_xPos;
		vector.m_yPos = m_yPos + v.m_yPos;

		return vector;
	}

	Vec2 SubVector(Vec2 &v)
	{
		Vec2 vector;
		vector.m_xPos = m_xPos - v.m_xPos;
		vector.m_yPos = m_yPos - v.m_yPos;

		return vector;
	}

	Vec2 MultiVector(float f)
	{
		m_xPos = m_xPos * f;
		m_yPos = m_yPos * f;

		return *this;
	}

	float GetVectorLength()
	{
		return sqrt((m_xPos * m_xPos) + (m_yPos * m_yPos));
	}

	Vec2 NormalizeVector()
	{
		float length = this->GetVectorLength();

		if (length != 0) {
			m_xPos = m_xPos / length;
			m_yPos = m_yPos / length;
		}

		return *this;
	}

	float GetDistanceTo(Vec2 &v)

	{
		float distance = sqrt(((v.m_xPos - m_xPos) * (v.m_xPos - m_xPos)) + ((v.m_yPos - m_yPos) * (v.m_yPos - m_yPos)));
		return distance;
	}

	Vec2 Vec3ToVec2(Vec3 &v)
	{
		Vec2 vector{ v.getxPos(), v.getyPos() };
		return vector;
	}

	Vec2 RotatePoint(Vec2 centerPoint, float angle, bool angleInRadians = false)
	{
		if (!angleInRadians)
		{
			angle = (angle * (static_cast<float>(M_PI) / 180.0f));
		}
		float cosTheta = cos(angle);
		float sinTheta = sin(angle);
		Vec2 returnVec = { cosTheta * (m_xPos - centerPoint.m_xPos) - sinTheta * (m_yPos - centerPoint.m_yPos),
			sinTheta * (m_xPos - centerPoint.m_xPos) + cosTheta * (m_yPos - centerPoint.m_yPos) };

		return (returnVec.AddVector(centerPoint));
	}
};

float GetDistance(Vec3 &v1, Vec3 &v2)

{
	float distance = sqrt(((v1.m_xPos - v2.m_xPos) * (v1.m_xPos - v2.m_xPos)) + ((v1.m_yPos - v2.m_yPos) * (v1.m_yPos - v2.m_yPos)) + ((v1.m_zPos - v2.m_zPos) * (v1.m_zPos - v2.m_zPos)));
	return distance;
}

float GetDistance(Vec2 &v1, Vec2 &v2)

{
	float distance = sqrt(((v1.m_xPos - v2.m_xPos) * (v1.m_xPos - v2.m_xPos)) + ((v1.m_yPos - v2.m_yPos) * (v1.m_yPos - v2.m_yPos)));
	return distance;
}