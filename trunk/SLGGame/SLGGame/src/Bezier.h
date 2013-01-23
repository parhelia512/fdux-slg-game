#pragma once
#include <math.h>

#include <Ogre.h>

class Bezier
{

public:
	//�������Ƶ�

	Ogre::Vector3 P0,P1,P2;

	float CurX,CurY;

	float DX,DY;

	int ax,ay,bx,by;

	double A,B,C;

	//�����ܳ���

	double total_length;

	void initValue()
	{
		total_length=0;

		ax = P0.x-2*P1.x+P2.x;

		ay = P0.y-2*P1.y+P2.y;

		bx = 2*P1.x-2*P0.x;

		by = 2*P1.y-2*P0.y;

		A = 4*(ax*ax+ay*ay);

		B = 4*(ax*bx+ay*by);

		C = bx*bx+by*by;
		
		total_length = L(1);
	}

	//-------------------------------------------------------------------------------------

	//�ٶȺ���

	/*

	s(t_) = Sqrt[A*t*t+B*t+C]

	*/

	double s(double t)
	{

		return sqrt(A*t*t+B*t+C);

	}

	//-------------------------------------------------------------------------------------

	//���Ⱥ���

	/*



	L(t) = Integrate[s[t], t]



	L(t_) = ((2*Sqrt[A]*(2*A*t*Sqrt[C + t*(B + A*t)] + B*(-Sqrt[C] + Sqrt[C + t*(B + A*t)])) + 

				(B^2 - 4*A*C) (Log[B + 2*Sqrt[A]*Sqrt[C]] - Log[B + 2*A*t + 2 Sqrt[A]*Sqrt[C + t*(B + A*t)]]))

					/(8* A^(3/2)));

	*/

	double L(double t)

	{

		double temp1 = sqrt(C+t*(B+A*t));

		double temp2 = (2*A*t*temp1+B*(temp1-sqrt(C)));

		double temp3 = log(B+2*sqrt(A)*sqrt(C));

		double temp4 = log(B+2*A*t+2*sqrt(A)*temp1);

		double temp5 = 2*sqrt(A)*temp2;

		double temp6 = (B*B-4*A*C)*(temp3-temp4);

		

		return (temp5+temp6)/(8*pow(A,1.5));

	}



	//-------------------------------------------------------------------------------------

	//���Ⱥ�����������ʹ��ţ�����߷����

	/*

		X(n+1) = Xn - F(Xn)/F'(Xn)

	*/

	double InvertL(double t, double l)

	{

		double t1=t, t2;

		do

		{

			t2 = t1 - (L(t1)-l)/s(t1);

			if(abs(t1-t2)<0.000001) break;

			t1=t2;

		}while(true);

		return t2;

	}

	void calculate(int s,int Step)
	{
			double t = (double)s/Step;

			//���������������,��ʱ��Ӧ�����߳���

			double l = t*total_length;

			//����L�����ķ����������l��Ӧ��tֵ

			t = InvertL(t, l);



			//���ݱ��������ߺ��������ȡ�ô�ʱ��x,y����

			double x = (1-t)*(1-t)*P0.x +2*(1-t)*t*P1.x + t*t*P2.x;

			double y = (1-t)*(1-t)*P0.y +2*(1-t)*t*P1.y + t*t*P2.y;



			double Q0X=(1 - t) * P0.x + t * P1.x;
			double Q0Y=(1 - t) * P0.y + t * P1.y;

			double Q1X=(1 - t) * P1.x + t * P2.x;
			double Q1Y=(1 - t) * P1.y + t * P2.y;

			DX = (Q1X - Q0X);
			DY = (Q1Y - Q0Y);

			//ȡ��

			CurX = x;

			CurY = y;
	}


};