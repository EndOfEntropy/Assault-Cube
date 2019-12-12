#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>

using namespace std;

class Vec3
{
private:
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

	void printVector()
	{
		cout << m_xPos << ' ' << m_yPos << ' ' << m_zPos << "\n";
	};

	Vec3 addVector(Vec3 &v)
	{
		Vec3 vector;
		vector.m_xPos = m_xPos + v.m_xPos;
		vector.m_yPos = m_yPos + v.m_yPos;
		vector.m_zPos = m_zPos + v.m_zPos;

		return vector;
	}

	Vec3 subVector(Vec3 &v)
	{
		Vec3 vector;
		vector.m_xPos = m_xPos - v.m_xPos;
		vector.m_yPos = m_yPos - v.m_yPos;
		vector.m_zPos = m_zPos - v.m_zPos;

		return vector;
	}

	Vec3 multiVector(float f)
	{
		Vec3 vector;		
		vector.m_xPos = m_xPos * f;
		vector.m_yPos = m_yPos * f;
		vector.m_zPos = m_zPos * f;

		return vector;
	}

	float getVectorLength()
	{
		return sqrt((m_xPos * m_xPos) + (m_yPos * m_yPos) + (m_zPos * m_zPos));
	}

	Vec3 normalizeVector()
	{
		Vec3 vector;
		float length = vector.getVectorLength();

		if (length != 0) {
			vector.m_xPos = m_xPos / length;
			vector.m_yPos = m_yPos / length;
			vector.m_zPos = m_zPos / length;
		}

		return vector;
	}

	float getDistanceTo(Vec3 &v)

	{
		float distance = sqrt(((v.m_xPos - m_xPos) * (v.m_xPos - m_xPos)) + ((v.m_yPos - m_yPos) * (v.m_yPos - m_yPos)) + ((v.m_zPos - m_zPos) * (v.m_zPos - m_zPos)));
		return distance;
	}

};


class Vec2
{
	private:
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

		void printVector()
		{
			cout << m_xPos << ' ' << m_yPos << ' ' << "\n";
		};

		Vec2 addVector(Vec2 &v)
		{
			Vec2 vector;
			vector.m_xPos = m_xPos + v.m_xPos;
			vector.m_yPos = m_yPos + v.m_yPos;

			return vector;
		}

		Vec2 subVector(Vec2 &v)
		{
			Vec2 vector;
			vector.m_xPos = m_xPos - v.m_xPos;
			vector.m_yPos = m_yPos - v.m_yPos;

			return vector;
		}

		Vec2 multiVector(float f)
		{
			m_xPos = m_xPos * f;
			m_yPos = m_yPos * f;

			return *this;
		}

		float getVectorLength()
		{
			return sqrt((m_xPos * m_xPos) + (m_yPos * m_yPos));
		}

		Vec2 normalizeVector()
		{
			float length = this->getVectorLength();

			if (length != 0) {
				m_xPos = m_xPos / length;
				m_yPos = m_yPos / length;
			}

			return *this;
		}

		float getDistanceTo(Vec2 &v)

		{
			float distance = sqrt(((v.m_xPos - m_xPos) * (v.m_xPos - m_xPos)) + ((v.m_yPos - m_yPos) * (v.m_yPos - m_yPos)));
			return distance;
		}

		Vec2 vec3ToVec2(Vec3 &v)
		{
			Vec2 vector{ v.getxPos(), v.getyPos() };
			return vector;
		}

		Vec2 RotatePoint(Vec2 centerPoint, float angle, bool angleInRadians = false)
		{
			if (!angleInRadians)
			{
				angle = (angle * (M_PI / 180.0f));
			}
			float cosTheta = cos(angle);
			float sinTheta = sin(angle);
			Vec2 returnVec = {	cosTheta * (m_xPos - centerPoint.m_xPos) - sinTheta * (m_yPos - centerPoint.m_yPos),
								sinTheta * (m_xPos - centerPoint.m_xPos) + cosTheta * (m_yPos - centerPoint.m_yPos)};

			return (returnVec.addVector(centerPoint));
		}
};