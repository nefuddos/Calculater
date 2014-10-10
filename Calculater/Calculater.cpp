// Calculater.cpp : 定义控制台应用程序的入口点。

#include "stdafx.h"
#include "Digit.h"
#include "function.h"


#ifndef FUNC_DEFINE
#define FUNC_DEFINE
#include "func_define.h"
#endif

#ifndef MEM
#define MEM
#include <memory.h>
#endif

using namespace std;

#ifndef SINT
#define SINT -1//如果返回值为这个数，说明字符窜接受有误，也就是说参数的传递有误，
#endif

#ifndef MAIN
#define MAIN
#define MAX 100
#define num 25
const char func_array[num][20]={"SIN","COS","TAN","ARCSIN","ARCCOS","ARCTAN","SINH","COSH","TANH","LOG10","LN","EXP","FACT","SQRT","CUBEROOT","LOG","POW","MOD","YROOT","AVG","SUM","VAR","VARP","STDEV","STDEVP"};//函数名字
typedef struct Fun{
 int canshu_number;//表示从字符窜中传递的参数的个数
 int std_number;//表示函数的标准参数的个数
 int bracket;//表示这个函数中括号中的个数
 int count;
 long double array[num];//接受参数数组
 char temp[MAX];
}fun;
fun function_array[num];
#endif
stack <int> func_stack;//函数栈
bool judge_space(char r)
{
	if(r>='0'&&r<='9'||r=='+'||r=='-'||r=='*'||r=='/'||r=='%'||r=='^'||r=='.')
		return true;
	else 
		return false;
}
bool judge(char r)
{
	if(r=='+'||r=='-'||r=='*'||r=='/'||r=='^')
		return true;
	else
		return false;
}
void init()//there is no problem
{
	int i,j;
	for(i=0;i<25;i++)
	{
		function_array[i].count=0;
		function_array[i].canshu_number=0;
		function_array[i].bracket=0;
		memset(function_array[i].temp,'\0',sizeof(function_array[i].temp));
			for(j=0;j<num;j++)
			{
				function_array[i].array[j]=0;
			}
		if(i<15)
			function_array[i].std_number=1;
		if(i>=15&&i<25)
			function_array[i].std_number=2;
	}
	//cout<<"init succeed! good luck!"<<endl;
}



char *zhuanhua_daxie(char *temp)
{
	//cout<<"translate into the lowercase"<<endl;
	int i,len=strlen(temp);
	for(i=0;i<len;i++)
	{
		if(!isupper(temp[i]))
			temp[i]=toupper(temp[i]);
	}
	return temp;
}

int look_find(char *temp)
{
	int i;
	for(i=0;i<num;i++)
	{
		if(strcmp(temp,func_array[i])==0)
		{
			//cout<<"find the function of number:"<<i+1<<endl;
			return i;//返回函数的编号
			
		}
		
	}
	if(i>=num)//否则，则输入有误
		return -1;
	
}



long double func(char *str)
{
	int i,cal_flag=0,k_digit=0,temp_len=0;
	int temp_dispose_flag=0;
	int len=strlen(str);
	int number;
	char str_copy[MAX];
	char temp_name[MAX],*p;
	int replase_flag=0;
	char receive[MAX];
	long  double result;
//	int count_array[MAX];
	memset(str_copy,'\0',MAX);
	memset(temp_name,'\0',MAX);
	memset(receive,'\0',MAX);

	for(i=0;i<len;)
	{
		//cout<<"the steps of"<<i<<endl;
		if((str[i]>='A'&&str[i]<='Z')||(str[i]>='a'&&str[i]<='z'))
		{
			
			  sscanf(str+i,"%[A-Z,a-z]",temp_name);
			  temp_len=strlen(temp_name);
			  i+=temp_len;
			  p=zhuanhua_daxie(temp_name);
		    //对于LOG10这种特殊情况的判断
			if(strcmp(p,"LOG")==0&&str[i]=='1'&&str[i+1]=='0')
			{
				temp_name[temp_len]='1';
				temp_name[temp_len+1]='0';
				p=zhuanhua_daxie(temp_name);
				i=i+2;
			}

			//对于mod为运算符的处理
			if(strcmp(p,"MOD")==0&&(str[i]>='0'&&str[i]<='9'))
			{
				replase_flag=1;
				memset(temp_name,'\0',MAX);
				goto  ddos;                                           
			}
			number=look_find(p);//返回函数的标号

			if(number==-1)
			{
			    //cout<<"输入有误"<<endl;
				return SINT;
			}
			func_stack.push(number);
			//cout<<"当前函数为"<<func_stack.top()<<endl;
			memset(temp_name,'\0',MAX);
			
			continue;
		}
		if(str[i]==']'||str[i]=='['||str[i]==' ')
		{
			if(i<len-1&&i>0&&str[i]==' '&&judge_space(str[i-1])&&judge_space(str[i+1]))//如果出现空格，而且空格的前后都是数字或则运算符，
				//那么就返回错误类型，但是在返回之前,应该将我建立的函数栈的信息清空
			{
				while(!func_stack.empty())
				  func_stack.pop();
                   return SINT;
			}
			i++;
			continue;
		}
		
		if(!func_stack.empty())//说明已经在进入某一个函数中
		{
			if(function_array[func_stack.top()].bracket==0&&str[i]=='(')
			{
				function_array[func_stack.top()].bracket++;//函数作用域的开始
				i++;
				continue;
			}

			if(function_array[func_stack.top()].bracket>=1&&str[i]!=')')
			{
				//if(str[i]>='0'&&str[i]<='9'||str[i]=='+'||str[i]=='-'||str[i]=='*'||str[i]=='/'||str[i]=='%'||str[i]=='^'||str[i]=='.')
				if(judge_space(str[i]))
				{
					function_array[func_stack.top()].temp[function_array[func_stack.top()].count++]=str[i];
					i++;
					continue;
				}
					
				if(str[i]=='(')
				{
					function_array[func_stack.top()].temp[function_array[func_stack.top()].count++]=str[i];
					function_array[func_stack.top()].bracket++;

temp_dispose: if(temp_dispose_flag==1)//处理参数为，字符串为
			  {
				 
					strcat(function_array[func_stack.top()].temp,receive);
					function_array[func_stack.top()].count+=strlen(receive);
					memset(receive,'\0',MAX);
					temp_dispose_flag=0;
			  }

				   
					i++;
					continue;
				}
				
				if(str[i]==')'&&function_array[func_stack.top()].bracket>1)
				{
					function_array[func_stack.top()].bracket--;
					
					function_array[func_stack.top()].temp[function_array[func_stack.top()].count++]=str[i];
					i++;
					continue;
				}

				if(str[i]==','&&function_array[func_stack.top()].temp[0]!='\0')//这里的判断是否正确呢？？
				{
					//cout<<function_array[func_stack.top()].temp<<endl;
					int l=strlen(function_array[func_stack.top()].temp);
					function_array[func_stack.top()].temp[l]='#';
					l++;
					result=string_temp_dispose(function_array[func_stack.top()].temp,l);//表达式处理函数

					memset(function_array[func_stack.top()].temp,'\0',MAX);
					//cout<<result<<endl;
					function_array[func_stack.top()].count=0;
					function_array[func_stack.top()].array[function_array[func_stack.top()].canshu_number++]=result;
			        i++;
			        continue;
				}

				/*
				if(str[i]==','&&temp[0]=='\0')
				{
					i++;
					continue;
				}

	           */
			}
			

			if(str[i]==')'&&function_array[func_stack.top()].bracket==1)
			{
				
				if(function_array[func_stack.top()].temp[0]!='\0')
				{
					//cout<<function_array[func_stack.top()].temp<<endl;
					int l=strlen(function_array[func_stack.top()].temp);
					function_array[func_stack.top()].temp[l]='#';
					l++;
					//cout<<"the temp is"<<function_array[func_stack.top()].temp<<endl;
					result = string_temp_dispose(function_array[func_stack.top()].temp,l);//表达式处理函数
					//cout<<"the result is"<<result<<endl;
					memset(function_array[func_stack.top()].temp,'\0',MAX);
					function_array[func_stack.top()].count=0;
					
					function_array[func_stack.top()].array[function_array[func_stack.top()].canshu_number++]=result;
			    
				}

				function_array[func_stack.top()].bracket--;
				if(function_array[func_stack.top()].bracket==0)//表示当前函数的结束
				{
						number=func_stack.top();
						func_stack.pop();
						
						result=function_dispose(number+1,function_array[number].array,function_array[number].canshu_number);
						//这里函数调用完以后需要将函数初始化
						//cout<<"the result of function is"<<result<<endl;

						memset(function_array[number].array,0,sizeof(function_array[number].array));
						function_array[number].canshu_number=0;
		
						if(result==SINT)
						{
							return SINT;
						}
						//从这一下需要修改！类型。。函数参数是数字加函数的类型
						if((str[i+1]==','||i+1>=len||str[i+1]==']')&&!func_stack.empty()&&function_array[func_stack.top()].temp[0]=='\0')
						{
							//cout<<"the result is"<<result<<endl;
							function_array[func_stack.top()].array[function_array[func_stack.top()].canshu_number++]=result;
						}
						else if(!func_stack.empty()&&(function_array[func_stack.top()].temp[0]!='\0'||judge(str[i+1])))
							{
								//cout<<"转化前的结果为："<<result<<endl;
								_gcvt((double)result,6,receive);
								//cout<<"转化为字符串的运算数字"<<receive<<endl;
								temp_dispose_flag=1;
								goto temp_dispose;
								
							}
						else{

							_gcvt((double)result,90,receive);
							

							int k_plus=strlen(receive);
							strcat(str_copy,receive);
							//cout<<k_plus<<endl;
							
							k_digit+=k_plus;
							
							//cout<<"the string is "<<str_copy<<' '<<k_digit<<endl;
							memset(receive,'\0',MAX);
						}
				}
				i++;
				
			}


		}//endif
		else
		{
		ddos:	if(replase_flag==1)
				{
				
				str_copy[k_digit++]='%';
				}
				
				//表明字符窜的一开始就是数字表达式，那么直接考入到str_copy中，
				str_copy[k_digit++]=str[i];
				i++;
				continue;
		}

	}//endfor
	str_copy[k_digit]='#';
	k_digit++;
	//cout<<"the string is "<<str_copy<<endl;
	result=string_temp_dispose(str_copy,k_digit);
	memset(str_copy,'\0',MAX);
	return result;
    
}
//
int precision(long double p)
{
	int n = 6 ;
	if(abs((int)( p * pow( 10.0 , n+1 ))) % 10 == 9)
	{
			for(n;n>0;){
				if(abs((int)(p*pow(10.0,n)))%10==9)
					n--;
				else break;
			}
	}

	else 
		for(n;n>0;)
		{
		    if((int)(p*pow(10.0,n))%10== 0 ) { 
			    n--;
		    }
		    else break;
	    }
    return n;
}

int main()
{
	cout<<"              科学计算器              "<<endl;
	cout<<"**************************************"<<endl;
	cout<<"请输入运算表达式(输入quit退出程序)："<<endl;
	char str[MAX];
	long double p;
	cin.getline(str,MAX);
	while(strcmp(str,"quit")!=0)//end up in quit!
	{
		init();
		p=func(str);
		if( p!=SINT )
			printf(" ans = %.*f\n",precision(p) ,p);
		else
			cout<<"输入数据有误!"<<endl;

      	memset(str,'\0',MAX);
		cout<<endl;
		//cout<<"请输入运算表达式(输入quit退出程序)："<<endl;
		cin.getline(str,MAX);
	}
	return 0;
}

