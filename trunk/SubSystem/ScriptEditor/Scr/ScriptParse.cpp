#include "ScriptParse.h"
#include <QFile>
#include <QMap>

ScriptParse::ScriptParse(void)
{
	QFile file("role.txt");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;
	
	Role r;
	int state=0;
	while (!file.atEnd()) 
	{
		QByteArray line = file.readLine();
		QString s(line);
		s=s.trimmed();

		if (s=="#")
		{
			state=1;
		}
		else if(state==1)
		{
			r.name=s;
			state=2;
		}
		else if (state==2 && s=="%%")
		{
			state=3;
		}
		else if(state==3)
		{
			if (s!="%%")
			{
				r.param.append(s);
			}
			else
			{
				state=4;
			}
		}
		else if(state==4)
		{
			r.lua=s;

			mRoles.append(r);

			r.param.clear();
		}
	}

	file.close();
}

ScriptParse::~ScriptParse(void)
{
}

QString ScriptParse::parse( QString text )
{
	mResult.clear();
	mError="";
	QString::Iterator it;
	bool isOrderBegin=false;
	bool isTextBegin=true;
	QString s;
	int line=0;

	for (it=text.begin();it!=text.end();it++)
	{
		if (QString((*it))=="[")
		{
			
			if (isOrderBegin)//˵����[û�պ�
			{
				mError=QString(line)+":������δ�պ�";
				return mError;
			}

			if (isTextBegin && !s.isEmpty())
			{
				mTextBuffer=s;//���ı����뻺����,������һ��ָ�������,������δ���
				s="";
			}
			isOrderBegin=true;
			isTextBegin=false;
		}
		else if (QString((*it))=="]")
		{
			
			if (!isOrderBegin)//˵����]��ǰ�պ�
			{
				mError=QString(line)+":������δ�պ�";
				return mError;
			}
			
			if (s.isEmpty())
			{
				mError=QString(line)+":������";
				return mError;
			}

			mResult.append(orderParse(s));

			if(!mError.isEmpty())
			{
				return QString().setNum(line)+":"+mError;
			}
			
			isOrderBegin=false;
			isTextBegin=true;
			s="";
		}
		else if (QString((*it))=="\n")
		{
			line++;
		}
		else
		{
			s+=(*it);
		}
	}

	return mResult.join("\n");
}

QString ScriptParse::orderParse( QString text )
{
	QStringList sl=text.trimmed().split(" ");

	if (sl.size()!=0)
	{
		QMap<QString,QString> valueMap;//��ȡ����
		for (int i=1;i<sl.size();i++)
		{
			QStringList param=sl.at(i).split("=");
			if (param.size()==2)
			{
				valueMap[param.at(0)]=param.at(1);
			}
			else
			{
				mError="������ʽ����ȷ";
				return "";
			}
		}

		if (!mTextBuffer.isEmpty())//����ı����������ж���,�ͼ�������б���ȥ
		{
			valueMap["TextBuffer"]=mTextBuffer;
		}
		
		bool flag=false;
		QString result;
		QVector<Role>::Iterator it;
		for (it=mRoles.begin();it!=mRoles.end();it++)
		{
			if ((*it).name==sl.at(0))//�������ƥ��
			{
				flag=true;
				result=(*it).lua;
				QStringList::Iterator sIt;
				for (sIt=(*it).param.begin();sIt!=(*it).param.end();sIt++)//�����������Ʊ�
				{
					QString value=valueMap.value((*sIt));//�Ӳ���������������;
					if (!value.isEmpty())
					{
						
						if ((*sIt)=="TextBuffer")
						{
							mTextBuffer.clear();//��ʾʹ�ù���mTextBuffer
						}

						result.replace((*sIt),value);//�滻��lua�еĲ�����Ϊ��������
					}
					else
					{
						mError="����";
						mError+=(*sIt)+"δ�ҵ�";
						return "";
					}
				}
				break;
			}
		}

		if (!flag)
		{
			mError="ָ��δ����";
			return "";
		}
		else
		{
			
			if (!mTextBuffer.isEmpty())//�����ж�mTextBuffer�Ƿ��Ѿ���������,û����Ļ���ʹ��Ĭ�ϴ���
			{
				mTextBuffer.insert(0,"GUI.ShowText(\"");
				mTextBuffer.append("\",1);\n");
				result.insert(0,mTextBuffer);
				mTextBuffer.clear();
			}

			return result;
		}
	}
	return "";
}

bool ScriptParse::hasError()
{
	if (!mError.isEmpty())
	{
		return true;
	}
	else
	{
		return false;
	}
}
