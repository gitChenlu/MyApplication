// cgm_target_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "stdlib.h"
#include <vector>
#include <list>
#include <math.h>
#include <algorithm>
#include <ATLComTime.h>
#include <cstring>
#include <string>
#include <numeric>
#include<tchar.h>


using namespace std;


/*
@brief: time（string） 转 COleDateTime
@param:
	input:时间字符串，string
@ret: COleDateTime时间，COleDateTime
*/
COleDateTime time2COleDateTime(string s_time)
{
	COleDateTime out = {};
	CString a = s_time.c_str();
	out.ParseDateTime((LPCTSTR)(a.GetBuffer()));
	return out;
}

/*
@brief: COleDateTime 转 time（string）
@param:
	input:COleDateTime时间，COleDateTime
@ret: 时间字符串，string
*/
string  COleDateTime2time(COleDateTime cdtime_time)
{
	CString cstr_time = cdtime_time.Format(_T("%Y-%m-%d %H:%M"));
	string out = CW2A(cstr_time.GetString());
	return out;
}

/*
@brief: 根据第一个时间戳，计算与标准时间戳的差值。
	计算逻辑：remainder = minutes % 5。
@param:
	input:
		start_datetime：第一个时间戳，string
@ret:
	remainder：根据第一个时间戳计算得到余数，int。
*/
int calc_remainder(string start_datetime)
{
	COleDateTime cdt_start = time2COleDateTime(start_datetime);
	int minute = cdt_start.GetMinute();
	int remainder = minute % 5;
	return remainder;
}

/*
 @brief: 根据第一个时间戳校正时间戳，计算时间戳编码。
			编码逻辑：将一天24小时按5分钟为间隔编码为0~287；00:00~00:04为0；00:05~00:09为1，以此类推。
			时间戳校正逻辑：round(h*60+m-remainder)/5.0
 @param:
		input:
			origin_datetime_array：输入时间数组，string*
			N：数组长度
			remainder：根据第一个时间戳计算得到余数，int；根据分数数值除以5，取余数。
		output:
			code_array:时间戳编码值数组,int*
			correct_datetime_array：校正后的时间戳,string *
 @ret: void
*/
void gen_timecode_correct_datetime(string * origin_datetime_array, int N, int remainder, int* code_array, string *correct_datetime_array)
{
	for (int i = 0; i < N; i++)
	{
		string temp_datetime = origin_datetime_array[i];
		CString temp_datetime_cstr = temp_datetime.c_str();
		COleDateTime cdt_datetime;

		cdt_datetime.ParseDateTime(temp_datetime_cstr);
		int ori_hour = cdt_datetime.GetHour();
		int ori_minute = cdt_datetime.GetMinute();
		int hm = ori_hour * 60 + ori_minute - remainder;
		float code1 = float(hm) / 5.0;
		int code2 = round(code1);
		int time_diff = code2 * 5 - hm;
		COleDateTimeSpan timespan(0, 0, time_diff, 0);
		if (code2 == -1)
		{
			code2 = 287;
		}
		else if (code2 == 288)
		{
			code2 = 0;
		}
		code_array[i] = code2;

		if (code2 == code1)
		{
			correct_datetime_array[i] = temp_datetime;
		}
		else
		{
			COleDateTime correct_datetime;
			correct_datetime = cdt_datetime + timespan;
			correct_datetime_array[i] = COleDateTime2time(correct_datetime);
		}
	}
}



/*
 @brief: 根据不同的糖尿病类型，将血糖值分类为极低(-2)、低(-1)、正常(0)、高(1)、极高(2)，我称之为血糖水平编码。
 @param:
		input:
			glucose：血糖值,float
			type：糖尿病类型。int,0：正常；1：1、2型糖尿病；2：妊娠糖；3：老年糖；4：自定义；默认正常。
			low:低血糖阈值（自定义值，仅在type为4时启用）,float
			high:高血糖阈值（自定义值，仅在type为4时启用）,float
 @ret: tir code,值为{-2，-1,0,1,2},int
*/
int gen_tir_code(float glucose, int type = 0, float low = 3.9, float high = 7.8)
{
	//	glucose = round(glucose * 10) / 10.0;
	int code = 0;
	switch (type)
	{
		case 0:
		{
			//			if (glucose <= 3.9)
			if (glucose - 3.9 < 0.0005)
				code = -1;
				//			else if (glucose < 7.8)
			else if (glucose - 7.8 < -0.0005)
				code = 0;
			else
				code = 1;
			break;
		}
		case 1:
		{
			//			if (glucose < 3.0)
			if (glucose - 3.0 < -0.005)
				code = -2;
				//			else if (glucose < 3.9)
			else if (glucose - 3.9 < -0.005)
				code = -1;
				//			else if (glucose <= 10.0)
			else if (glucose - 10.0 < 0.005)
				code = 0;
				//			else if (glucose <= 13.9)
			else if (glucose - 13.9 < 0.005)
				code = 1;
			else
				code = 2;
			break;
		}
		case 2:
		{
			//			if (glucose < 3.0)
			if (glucose - 3.0 < -0.005)
				code = -2;
				//			else if (glucose < 3.5)
			else if (glucose - 3.5 < -0.005)
				code = -1;
				//			else if (glucose <= 7.8)
			else if (glucose - 7.8 < 0.005)
				code = 0;
			else
				code = 1;
			break;
		}
		case 3:
		{
			//			if (glucose < 3.9)
			if (glucose - 3.9 < -0.005)
				code = -1;
				//			else if (glucose <= 10.0)
			else if (glucose - 10.0 < 0.005)
				code = 0;
				//			else if (glucose <= 13.9)
			else if (glucose - 13.9 < 0.005)
				code = 1;
			else
				code = 2;
			break;
		}
		case 4:
		{
			//			if(glucose <= low)
			if (glucose - low < 0.005)
				code = -1;
				//			else if (glucose < high)
			else if (glucose - high < -0.005)
				code = 0;
			else
				code = 1;
			break;
		}
		default:
		{
			//			if (glucose <= 3.9)
			if (glucose - 3.9 < 0.0005)
				code = -1;
				//			else if (glucose < 7.8)
			else if (glucose - 7.8 < -0.0005)
				code = 0;
			else
				code = 1;
			break;
		}
	}
	return code;
}

/*
@brief: 在gen_tir_code函数的基础上处理数组
@param:
	input:
		glucose：血糖值,float*
		type：糖尿病类型。0：正常；1：1、2型糖尿病；2：妊娠糖；3：老年糖；4：自定义；默认正常。
		low:低血糖阈值（自定义值，仅在type为4时启用）
		high:高血糖阈值（自定义值，仅在type为4时启用）
@ret: tir_code_array,int *
*/
int* gen_tir_code_array(float* glucose, int N, int type = 0, float low = 3.9, float high = 7.8)
{
	//	int N = length(glucose);
	int *tir_code_array = new int[N];
	for (int i = 0; i < N; i++)
	{
		tir_code_array[i] = gen_tir_code(glucose[i], type, low, high);
	}
	return tir_code_array;
}



/*
@brief: 计算均值。
@param:
	input:
		bg_array：血糖值数组，float*
		N:数组长度，int
@ret: 血糖均值,float
*/

float calc_mean(float *bg_array, int N)
{
	float mean_value = 0;
	float sum_value = 0;
	//	int N = length(bg);
	for (int i = 0; i < N; i++)
	{
		sum_value += bg_array[i];
	}
	if (sum_value != 0)
		mean_value = sum_value / N;
	return mean_value;
}

/*
@brief: 计算标准差。
@param:
	input:
		bg_array：血糖值数组，float*
		N:数组长度，int
@ret: 血糖标准差，float
*/

float calc_std(float *bg, int N)
{
	float mean_value = calc_mean(bg, N);
	float var_sum = 0;
	float std = 0;
	for (int i = 0; i < N; i++)
	{
		var_sum += (bg[i] - mean_value)*(bg[i] - mean_value);
	}
	if (var_sum != 0)
		std = sqrt(var_sum / (N - 1));
	return std;

}


/*
@brief: 平均血糖值转换为预估糖化血红蛋白。
@param:
	input:
		mg：平均血糖,float
@ret: 预估糖化血红蛋白值,float
*/
float mg2eA1c(float mg)
{
	float eA1c = (mg + 0.582) / 1.198;
	return eA1c;
}


/*
@brief: 平均血糖值转换为葡萄糖管理指标。
@param:
	input:
		mg：平均血糖,float
		flag: 血糖单位，0为mg/dL,1为mmol/L，默认为1。int
@ret: 葡萄糖管理指标值,float
*/
float mg2gmi(float mg, int flag = 1)
{
	float gmi = 0;
	if (flag == 0)
		gmi = 3.31 + 0.02392*mg;
	else
		gmi = 12.71 + 4.70587*mg;
	return gmi;
}


/*
@brief: 计算AUCmd。
@param:
	input:
		mg_median_array：中位数血糖数组,float*
		N：数组长度，默认288，int
@ret: AUCmd,float
*/

float calc_AUCmd(float *bg_median_array, int N = 288)
{
	return calc_mean(bg_median_array, N);
}

/*
@brief: 计算变异系数。
@param:
	input:
		sdbg：血糖标准差,float
		mg：平均血糖,float
@ret: 变异系数,float
*/
float calc_cv(float sdbg, float mg)
{
	return sdbg / mg;
}


/*
@brief: 计算平均血糖波动幅度。
@param:
	input:
		bg_array：血糖数组，float*
		N:数组长度，int
		threshold：阈值，一般取24h血糖标准差
@ret: 平均血糖波动幅度,float
*/

float calc_mage(float *bg_array, int N, float threshold)
{
	vector<float>peaks_effective = {};
	int flag_arrow = 0;
	vector<int> idx;

	float temp_max = 0;
	float temp_min = 0;
	if (bg_array[0] > bg_array[1])
	{
		temp_max = bg_array[0];
		temp_min = bg_array[1];
	}
	else
	{
		temp_max = bg_array[1];
		temp_min = bg_array[0];
	}
	for (int i = 1; i < N; i++)
	{
		float now = bg_array[i];
		if (flag_arrow == 0)
		{
			if (temp_max == temp_min)
			{
				if (now > temp_max)
				{
					temp_max = now;
				}
				if (now < temp_min)
				{
					temp_min = now;
				}
			}
			else
			{
				if (temp_min > now)
				{
					temp_min = now;
				}
				if (temp_max < now)
				{
					temp_max = now;
				}
				if ((temp_max - temp_min) >= threshold)
				{
					if (now == temp_max)
					{
						flag_arrow = 1;
						peaks_effective.push_back(temp_min);
						peaks_effective.push_back(now);
					}
					else
					{
						flag_arrow = -1;
						peaks_effective.push_back(temp_max);
						peaks_effective.push_back(now);
					}
					idx.push_back(0);
					idx.push_back(i);
				}
			}
		}
		else
		{
			float before = peaks_effective[peaks_effective.size() - 1];
			if (flag_arrow == -1)
			{
				if ((now - before) >= threshold)
				{
					peaks_effective.push_back(now);
					idx.push_back(i);
					flag_arrow = -flag_arrow;
				}
				else
				{
					if ((now - before) < 0)
					{
						peaks_effective.pop_back();
						peaks_effective.push_back(now);
						idx.pop_back();
						idx.push_back(i);
					}
				}
			}
			if (flag_arrow == 1)
			{
				if ((before - now) >= threshold)
				{
					peaks_effective.push_back(now);
					idx.push_back(i);
					flag_arrow = -flag_arrow;
				}
				else
				{
					if ((before - now) < 0)
					{
						peaks_effective.pop_back();
						peaks_effective.push_back(now);
						idx.pop_back();
						idx.push_back(i);
					}
				}
			}
		}
	}

	float temp_sum = 0;
	for (int i = 1; i < peaks_effective.size(); i = i + 2)
	{
		temp_sum += abs(peaks_effective[i] - peaks_effective[i - 1]);
	}
	float mage = 0;
	if (peaks_effective.size() >= 2)
	{
		mage = temp_sum / (int(peaks_effective.size() / 2));
	}

	return mage;
}

/*
@brief: 计算最大血糖波动幅度。
@param:
	input:
		bg_array：血糖数组，float*
		N:数组长度,int
@ret: 最大血糖波动幅度,float
*/
float calc_lage(float *bg_array, int N)
{
	float min = bg_array[0];
	float max = bg_array[0];
	for (int i = 1; i < N; i++)
	{
		float now = bg_array[i];
		if (now > max)
			max = now;
		if (now < min)
			min = now;
	}
	return max - min;
}

/*
@brief: 计算日间血糖波动幅度。
@param:
	input:
		bg_current_array：当前血糖数据，float*
		code_current_array：当前时间编码数据,int*
		N_current：当前数据长度，int
		bg_ref_array：参考血糖数据，float*
		code_ref_array：参考时间编码数据,int*
		N_ref：参考数据长度,int
@ret: modd,float
*/
float calc_modd(float *bg_current_array, int *code_current_array, int N_current, float *bg_ref_array, int *code_ref_array, int N_ref)
{
	vector<float>current = {};
	vector<float>ref = {};
	for (int i = 0; i < N_current; i++)
	{
		int code = code_current_array[i];
		float glucose = bg_current_array[i];
		for (int j = 0; j < N_ref; j++)
		{
			int code_ref = code_ref_array[j];
			float glucose_ref = bg_ref_array[j];
			if (code == code_ref)
			{

				current.push_back(glucose);
				ref.push_back(glucose_ref);
				break;
			}
		}
	}
	float diff_sum = 0;
	for (int i = 0; i < current.size(); i++)
	{
		diff_sum += abs(current[i] - ref[i]);
	}
	float modd = diff_sum / current.size();
	return modd;
}



/*
@brief: 数据点计数转换为时间长度。
@param:
	input:
		n：数据点个数,int
@ret: 时间段,string
*/
string num2dur(int n)
{
	//	int day = int(n / 288);
	//	int day_rem = n % 288;
	int hour = int(n / 12);
	int minute = (n % 12) * 5;

	CString dur_cstr;

	dur_cstr.Format(_T("%d hours %d minutes"), hour, minute);
	//	dur_cstr = CString((LPCTSTR)day) + " " + CString((LPCTSTR)hour) + ":" + CString((LPCTSTR)minute);
	string dur_str = CW2A(dur_cstr.GetString());
	return dur_str;
}

/*
@brief: 计算tir。
@param:
	input:
		code_array：血糖水平编码数组，int*
		N：数组长度，int
	output:
		tir:tir百分比，float*,tir[0]对应极低，tir[1]对应低，tir[2]对应正常，tir[3]对应高，tir[4]对应极高，
		tir_time:tir时间长度,string*,同tir
@ret: void
*/
void calc_tir(int *code_array, int N, float *tir, string *tir_time)
{
	vector<int> tir_counts = { 0,0,0,0,0 };
	for (int i = 0; i < N; i++)
	{
		tir_counts[code_array[i] + 2]++;
	}
	tir[0] = float(tir_counts[0]) / N;
	tir[1] = float(tir_counts[1]) / N;
	tir[2] = float(tir_counts[2]) / N;
	tir[3] = float(tir_counts[3]) / N;
	tir[4] = float(tir_counts[4]) / N;

	tir_time[0] = num2dur(tir_counts[0]);
	tir_time[1] = num2dur(tir_counts[1]);
	tir_time[2] = num2dur(tir_counts[2]);
	tir_time[3] = num2dur(tir_counts[3]);
	tir_time[4] = num2dur(tir_counts[4]);
}


/*
@brief: tircode转换，便于血糖高和极高，低和极低时计算不冲突。(-2转为-1,2转为1)
@param:
input:
	tir_code：tir_code
@ret: 转化后的code
*/
int tir_code_trans(int tir_code)
{
	if (tir_code == 2)
		tir_code = 1;
	if (tir_code == -2)
		tir_code = -1;
	return tir_code;
}

/*
@brief: tircode转换，便于血糖高和极高，低和极低时计算不冲突。(-1转为0,1转为0)
@param:
	input:
		tir_code：tir_code
@ret: 转化后的code
*/
int tir_code_trans2(int tir_code)
{
	if (tir_code == 1)
		tir_code = 0;
	if (tir_code == -1)
		tir_code = 0;
	return tir_code;
}

/*
@brief: 计算血糖事件，两个测量点时间超过（等于）10分钟（即15分钟），记为1次血糖事件
@param:
	input:
		tir_code_array：血糖水平数组，int*
		correct_datetime_array:时间戳数组，string*
		N:数组长度,int
	output:
		tir_type:血糖水平数组，int*
		start:开始时间,string*
		end:结束时间,string*
@ret: 异常血糖事件计数，int
*/
int calc_event(int *tir_code_array, string *correct_datetime_array, int N, int *tir_type_array, string *start_array, string *end_array)
{
	//int N = sg1.size();
//	vector<int>v_tir_type = {};
//	vector<COleDateTime>v_start = {};
//	vector<COleDateTime>v_end = {};

	int num = 0;
	if (N != 0)
	{
		int start_code = tir_code_trans(tir_code_array[0]);
		string start_time_s = correct_datetime_array[0];
		string end_time_s = correct_datetime_array[0];
		COleDateTime start_time = time2COleDateTime(start_time_s);
		COleDateTime end_time = time2COleDateTime(end_time_s);
		COleDateTimeSpan dur;
		dur.SetDateTimeSpan(0, 0, 0, 0);

		int start_code2 = tir_code_trans2(tir_code_array[0]);
		string start_time2_s = correct_datetime_array[0];
		string end_time2_s = correct_datetime_array[0];
		COleDateTime start_time2 = time2COleDateTime(start_time2_s);;
		COleDateTime end_time2 = time2COleDateTime(end_time2_s);
		COleDateTimeSpan dur2;
		dur2.SetDateTimeSpan(0, 0, 0, 0);

		COleDateTimeSpan dur_ref;
		dur_ref.SetDateTimeSpan(0, 0, 10, 0);

		for (int i = 1; i < N; i++)
		{
			string now_time_s = correct_datetime_array[i];
			COleDateTime now_time = time2COleDateTime(now_time_s);

			int now_code = tir_code_trans(tir_code_array[i]);
			int now_code2 = tir_code_trans2(tir_code_array[i]);

			if (start_code == 0)
			{
				start_code = now_code;
				start_time = now_time;
				start_time_s = now_time_s;
				dur.SetDateTimeSpan(0, 0, 0, 0);
			}
			else
			{
				if (start_code == now_code)
				{
					end_time = now_time;
					end_time_s = now_time_s;
					dur = end_time - start_time;
				}
				else
				{
					if (dur >= dur_ref)
					{
						tir_type_array[num] = start_code;
						start_array[num] = start_time_s;
						end_array[num] = end_time_s;
						num++;
					}
					start_code = now_code;
					start_time = now_time;
					start_time_s = now_time_s;
					dur.SetDateTimeSpan(0, 0, 0, 0);
				}
			}

			//二级高低血糖
			if (start_code2 == 0)
			{
				start_code2 = now_code2;
				start_time2 = now_time;
				start_time2_s = now_time_s;
				dur2.SetDateTimeSpan(0, 0, 0, 0);
			}
			else
			{
				if (start_code2 == now_code2)
				{
					end_time2 = now_time;
					end_time2_s = now_time_s;
					dur2 = end_time2 - start_time2;
				}
				else
				{
					if (dur2 >= dur_ref)
					{
						tir_type_array[num] = start_code2;
						start_array[num] = start_time2_s;
						end_array[num] = end_time2_s;
						num++;
					}
					start_code2 = now_code2;
					start_time2 = now_time;
					start_time2_s = now_time_s;
					dur2.SetDateTimeSpan(0, 0, 0, 0);
				}
			}
		}
		if (start_code != 0)
		{
			if (dur >= dur_ref)
			{
				tir_type_array[num] = start_code;
				start_array[num] = start_time_s;
				end_array[num] = end_time_s;
				num++;
			}
		}
		if (start_code2 != 0)
		{
			if (dur2 >= dur_ref)
			{
				tir_type_array[num] = start_code;
				start_array[num] = start_time_s;
				end_array[num] = end_time_s;
				num++;
			}
		}
	}
	return num;
}

/*
@brief: 计算低血糖风险指数、高血糖风险指数
@param:
	input:
		bg_array：血糖数组
		N：数组长度
	output:
		lbgi:低血糖风险指数
		hbgi:高血糖风险指数
@ret: void
*/

void calc_lbgi_hbgi(float *bg_array, int N, float &lbgi, float &hbgi)
{
	vector<float> fbg = {};
	vector<float> rbg = {};
	vector<float> rlbg = {};
	vector<float> rhbg = {};

	float rlbg_sum = 0;
	float rhbg_sum = 0;
	for (int i = 0; i < N; i++)
	{
		float now_bg = bg_array[i];
		float now_fbg = 1.794*(pow(log(now_bg), 1.026) - 1.861);
		float now_rbg = 10 * now_fbg * now_fbg;
		fbg.push_back(now_fbg);
		rbg.push_back(now_rbg);
		if (now_fbg < 0)
		{
			rlbg.push_back(now_rbg);
			rlbg_sum += now_rbg;
		}

		if (now_fbg > 0)
		{
			rhbg.push_back(now_rbg);
			rhbg_sum += now_rbg;
		}

	}
	lbgi = rlbg_sum / rlbg.size();
	hbgi = rhbg_sum / rhbg.size();
}


/*
@brief: 升序排序，直接对原数组排序
@param:
	input:
		bg_array：血糖数组，float*
		N：数组长度,int
	output:
		in_array：直接对原数组排序，float*
@ret: void
*/
void sort_value(float *in_array, int N)
{
	for (int i = 0; i < N - 1; ++i)
	{
		int f = 1;
		for (int j = 0; j < N - i - 1; ++j)
		{
			if (in_array[j] > in_array[j + 1])
			{
				float t = in_array[j];
				in_array[j] = in_array[j + 1];
				in_array[j + 1] = t;
				f = 0;
			}
		}
		if (f == 1)
			break;
	}
}

/*
@brief: 计算分位数
@param:
	input:
		bg_t_array：连续时间内每天t时刻的血糖数组，float*
		p：分位点*100，如果是25%分位数，则p=25,int
@ret: 分位数，float
*/
float percentile(float *bg_t_array, int N, int p)
{
	sort_value(bg_t_array, N);
	float p1 = p / 100.0;
	float idx = p1*(N - 1);
	float idx_below_d = floor(idx);
	int idx_below = (int)(idx_below_d);
	int idx_above = idx_below + 1;
	float weights_above = idx - idx_below;
	float weights_below = 1.0 - weights_above;
	float v_below = bg_t_array[idx_below];
	float v_above = bg_t_array[idx_above];
	float value = v_below*weights_below + v_above*weights_above;
	return value;
}


/*
@brief: 将数据按照时间戳分组
@param:
	input:
		bg_array：血糖数组，float*
		code_array：时间戳编码数组，int*
		N:数组长度,int
	output：
		bg_array_group：分组结果，为288*天数，float**
		code_len_array：每个时间点（编码值）对应的数据个数，int*，长度为288
@ret: void
*/
void bg_group_time(float *bg_array, int *code_array, int N, float** bg_array_group, int* code_len_array)
{
	for (int i = 0; i < 288; i++)
		code_len_array[i] = 0;
	for (int i = 0; i < N; i++)
	{
		int code = code_array[i];
		int col = code_len_array[code];
		bg_array_group[code][col] = bg_array[i];
		code_len_array[code]++;
	}
}

/*
@brief: 根据分好的组，计算agp的5条曲线
@param:
	input:
		bg_array：血糖数组，float*
		code_array：时间戳编码数组，int*
		N:数组长度，int
	output：
		per5:5%血糖数组，float*
		per25:25%血糖数组，float*
		per50:50%血糖数组，float*
		per75:75%血糖数组，float*
		per95:95%血糖数组，float*
@ret: void
*/
void gen_agp(float *bg_array, int *code_array, int N, float *per5_array, float *per25_array, float *per50_array, float *per75_array, float *per95_array)
{
	float** group_array = new float *[288];
	for (int i = 0; i < 288; i++)
	{
		group_array[i] = new float[14];
	}
	int *code_len_array = new int[288];
	//	int * code_len_array = new int[288];
	bg_group_time(bg_array, code_array, N, group_array, code_len_array);
	for (int i = 0; i < 288; i++)
	{
		float *temp_group = group_array[i];
		int code_N = code_len_array[i];
		per5_array[i] = percentile(temp_group, code_N, 5);
		per25_array[i] = percentile(temp_group, code_N, 25);
		per50_array[i] = percentile(temp_group, code_N, 50);
		per75_array[i] = percentile(temp_group, code_N, 75);
		per95_array[i] = percentile(temp_group, code_N, 95);
		delete[] temp_group;
	}
	delete[] group_array;
	delete[] code_len_array;
}

/*
@brief: kalman滤波
@param:
	input:
		bg_array：输入的血糖值数组（分位数数组），float*
		N:数组长度
@ret: 滤波后的数据，float*
*/
float* agp_kalman_filter(float *bg_array, int N)
{
	float *bg_filtered_array = new float[N];
	float agp_state_kalman = 0.0;
	float agp_state_pre = 0.0;
	float agp_Pminus = 0.0;
	float agp_K = 0.0;
	float agp_R = 1.2;
	float agp_Q = 0.5;
	float agp_P = 0.01;

	for (int i = 0; i < N; i++)
	{
		float data = bg_array[i];
		if (i > 1)
		{
			agp_state_pre = agp_state_kalman;
			agp_Pminus = agp_P + agp_Q;
			agp_K = agp_Pminus / (agp_Pminus + agp_R);
			agp_state_kalman = agp_state_pre + agp_K*(data - agp_state_pre);
			agp_P = (1 - agp_K)*agp_Pminus;
		}
		else
		{
			agp_state_kalman = data;
		}
		bg_filtered_array[i] = agp_state_kalman;
	}
	return bg_filtered_array;
}

/*
@brief: 数据反转
@param:
	input:
		bg_array：输入的血糖值数组（分位数数组），float*
		N:数组长度
@ret: 反转后的数据
*/
float * Reversal_function(float *bg_array, int N)
{
	float *bg_reversal_array = new float[N];
	for (int i = 0; i < N; i++)
	{
		bg_reversal_array[N - i - 1] = bg_array[i];
	}
	return bg_reversal_array;
}


/*
@brief: 对agp曲线滤波,正反重复滤波14次
@param:
	bg_array：输入的血糖值数组（分位数数组），float*
	N:数组长度
@ret: 滤波后的数据
*/
float* agp_filter(float *bg_array, int N)
{
	float* bg_filtered_array = new float[N];
	bg_filtered_array = bg_array;
	bg_filtered_array = Reversal_function(bg_filtered_array, N);
	bg_filtered_array = agp_kalman_filter(bg_filtered_array, N);
	bg_filtered_array = Reversal_function(bg_filtered_array, N);
	bg_filtered_array = agp_kalman_filter(bg_filtered_array, N);
	bg_filtered_array = Reversal_function(bg_filtered_array, N);
	bg_filtered_array = agp_kalman_filter(bg_filtered_array, N);
	bg_filtered_array = Reversal_function(bg_filtered_array, N);
	bg_filtered_array = agp_kalman_filter(bg_filtered_array, N);
	bg_filtered_array = Reversal_function(bg_filtered_array, N);
	bg_filtered_array = agp_kalman_filter(bg_filtered_array, N);
	bg_filtered_array = Reversal_function(bg_filtered_array, N);
	bg_filtered_array = agp_kalman_filter(bg_filtered_array, N);
	bg_filtered_array = Reversal_function(bg_filtered_array, N);
	bg_filtered_array = agp_kalman_filter(bg_filtered_array, N);
	bg_filtered_array = Reversal_function(bg_filtered_array, N);
	bg_filtered_array = agp_kalman_filter(bg_filtered_array, N);
	bg_filtered_array = Reversal_function(bg_filtered_array, N);
	bg_filtered_array = agp_kalman_filter(bg_filtered_array, N);
	bg_filtered_array = Reversal_function(bg_filtered_array, N);
	bg_filtered_array = agp_kalman_filter(bg_filtered_array, N);
	bg_filtered_array = Reversal_function(bg_filtered_array, N);
	bg_filtered_array = agp_kalman_filter(bg_filtered_array, N);
	bg_filtered_array = Reversal_function(bg_filtered_array, N);
	bg_filtered_array = agp_kalman_filter(bg_filtered_array, N);
	bg_filtered_array = Reversal_function(bg_filtered_array, N);
	bg_filtered_array = agp_kalman_filter(bg_filtered_array, N);
	bg_filtered_array = Reversal_function(bg_filtered_array, N);
	bg_filtered_array = agp_kalman_filter(bg_filtered_array, N);

	return bg_filtered_array;

}

/*
@brief: 将字符串根据指定的字符分割
@param:
	input:
		s：输入的字符串
		seperator：分割的字符串标志
@ret: 分割得到的字符串vector
*/
vector<string> split(const string &s, const string &seperator) {
	vector<string> result;
	typedef string::size_type string_size;
	string_size i = 0;

	while (i != s.size()) {
		//找到字符串中首个不等于分隔符的字母；
		int flag = 0;
		while (i != s.size() && flag == 0) {
			flag = 1;
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[i] == seperator[x]) {
					++i;
					flag = 0;
					break;
				}
		}

		//找到又一个分隔符，将两个分隔符之间的字符串取出；
		flag = 0;
		string_size j = i;
		while (j != s.size() && flag == 0) {
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[j] == seperator[x]) {
					flag = 1;
					break;
				}
			if (flag == 0)
				++j;
		}
		if (i != j) {
			result.push_back(s.substr(i, j - i));
			i = j;
		}
	}
	return result;
}


/*
@brief: 按天将数据分组
@param:
	input:
		sg1：输入stru_glucose结构体vector
@ret: 按天的数据 vector
*/
int group_by_day(float *bg_array, int N, int *code_array, string *datetime_array, float **bg_group_array, int **code_group_array, string **datetime_group_array, int *day_len_array)
{
	COleDateTime start_time = time2COleDateTime(datetime_array[0]);
	COleDateTime end_time = time2COleDateTime(datetime_array[N - 1]);

	COleDateTime start_day(start_time.GetYear(), start_time.GetMonth(), start_time.GetDay(), 0, 0, 0);
	COleDateTime end_day(end_time.GetYear(), end_time.GetMonth(), end_time.GetDay(), 0, 0, 0);
	COleDateTimeSpan dur = end_day - start_day;
	int dur_days = dur.GetDays()+1;

	for (int i = 0; i < dur_days; i++)
		day_len_array[i] = 0;

	for (int i = 0; i < N; i++)
	{
		COleDateTime now = time2COleDateTime(datetime_array[i]);
		COleDateTimeSpan dur_now = now - start_day;
		int group_idx = dur_now.GetDays();
		int col = day_len_array[group_idx];

		bg_group_array[group_idx][col] = bg_array[i];
		code_group_array[group_idx][col] = code_array[i];
		datetime_group_array[group_idx][col] = datetime_array[i];
		day_len_array[group_idx]++;
	}
	return dur_days;
}



/*
@brief: 从txt文本从读取数据
@param:
input:
filename：文件名
output:
bg_array：血糖数组,float *
idx_array：索引值数组，int *
origin_datetime_array:时间戳数组,string*
num：数组长度
@ret: void
*/

void Read_Data(const char *filename, float *bg_array, int *idx_array, string *origin_datetime_array, int &num)
{
	ifstream in(filename);
	string line;
	num = 0;
	if (in)
	{
		while (getline(in, line))
		{
			vector<string> aa = split(line, "\t");
			int idx = atoi(aa[0].c_str());
			string timestamp = aa[1].c_str();
			float glucose = atof(aa[2].c_str());
			if (idx % 5 == 0)
			{
				idx_array[num] = idx;
				origin_datetime_array[num] = timestamp;
				bg_array[num] = glucose;
				num++;
			}
		}
	}
	in.close();
}


int main()
{

	const char *filename = "E:\\vsPro\\cgm_target_test\\cgm_target_test\\LT210102A6s.txt";//本地文件
	ofstream out("target_update_3.txt");//输出文件
	int type = 3;//糖尿病类型 **********

	float *bg_array = new float[4032];//存放血糖值
	int *idx_array = new int[4032];//存放index
	string *origin_datetime_array = new string[4032];//存放时间戳
	int num = 0;//数据长度
	Read_Data(filename, bg_array, idx_array, origin_datetime_array, num);//读取数据

	float *bg_nm_array = new float[num - 12];//去除前1小时数据 *******
	string *origin_datetime_nm_array = new string[num - 12];//去除前1小时数据
	int num_nm = num - 12;//去除前1小时数据
	for (int i = 0; i < num - 12; i++)
	{
		bg_nm_array[i] = bg_array[i + 12];
		origin_datetime_nm_array[i] = origin_datetime_array[i + 12];
	}

	int remainder = calc_remainder(origin_datetime_nm_array[0]);//与标准时间戳的差值

	int *code_array = new int[num_nm];//存放时间戳编码code *********
	string *correct_datetime_array = new string[num_nm];//存放校正后的时间 ******
	gen_timecode_correct_datetime(origin_datetime_nm_array, num_nm, remainder, code_array, correct_datetime_array);

	int *tir_code_array = gen_tir_code_array(bg_nm_array, num_nm, type);//血糖水平编码 *********


	//按天分组
	float **bg_group_array = new float*[15]; //血糖分组数据
	int **code_group_array = new int*[15]; //时间戳编码分组数据
	string **datetime_group_array = new string*[15];//校正后时间戳 分组数据
	int *day_len_array = new int[15]; //每天数据长度
	for (int i = 0; i < 15; i++)
	{
		bg_group_array[i] = new float[288];
		code_group_array[i] = new int[288];
		datetime_group_array[i] = new string[288];
	}
	//day_count为天数
	int day_count = group_by_day(bg_nm_array, num_nm, code_array, correct_datetime_array, bg_group_array, code_group_array, datetime_group_array, day_len_array);

	for (int day = 0; day < day_count; day++)
	{
		int day_len = day_len_array[day];//第day天的数据长度
		float *bg_day_array = bg_group_array[day]; //第day天的血糖数据
		int *code_day_array = code_group_array[day];//第day天的时间戳编码数据
		string *datetime_day_array = datetime_group_array[day];//第day天的校正后时间戳

		float mg = calc_mean(bg_day_array, day_len);//均值
		float sdbg = calc_std(bg_day_array, day_len);//标准差
		float ea1c = mg2eA1c(mg);//ea1c
		float gmi = mg2gmi(mg);//gmi
		float cv = calc_cv(sdbg, mg);//cv
		float mage = calc_mage(bg_day_array, day_len, sdbg);//mage
		float lage = calc_lage(bg_day_array, day_len);//lage
		//modd
		float modd = 0;
		if (day > 0)
		{
			float *bg_day_ref_array = bg_group_array[day - 1];
			int *code_day_ref_array = code_group_array[day - 1];
			int day_ref_len = day_len_array[day - 1];
			modd = calc_modd(bg_day_array, code_day_array, day_len, bg_day_ref_array, code_day_ref_array, day_ref_len);
		}
		float lbgi = 0, hbgi = 0;
		calc_lbgi_hbgi(bg_day_array, day_len, lbgi, hbgi);//lbgi,hbgi

		int *tir_code_day_array = gen_tir_code_array(bg_day_array, day_len, type);//血糖水平编码
		float *tir = new float[5];
		string *tir_time = new string[5];
		calc_tir(tir_code_day_array, day_len, tir, tir_time);//tir

		int *tir_day_type = new int[(int)(day_len / 3)];
		string * start_day_array = new string[(int)(day_len / 3)];
		string * end_day_array = new string[(int)(day_len / 3)];
		int event_day_num = calc_event(tir_code_day_array, datetime_day_array, day_len, tir_day_type, start_day_array, end_day_array);//血糖事件


		out << "day: " << day << "\n";
		out << "mg: " << mg << "\n";
		out << "eA1c: " << ea1c << "\n";
		out << "gmi: " << gmi << "\n";
		out << "sdbg: " << sdbg << "\n";
		out << "cv: " << cv << "\n";
		out << "mage: " << mage << "\n";
		out << "lage: " << lage << "\n";
		out << "modd: " << modd << "\n";
		out << "lbgi: " << lbgi << "\n";
		out << "hbgi: " << hbgi << "\n";
		out << "\n";

		out << "tir" << "\n";
		for (int i = 0; i < 5; i++)
		{
			out << i - 2 << "\t" << tir[i] << "\t" << tir_time[i] << endl;
		}
		out << "\n" << "event" << "\n";
		for (int i = 0; i < event_day_num; i++)
		{
			out << i << "\t" << tir_day_type[i] << "\t" << start_day_array[i] << "\t" << end_day_array[i] << "\n";
		}
	}

	float *per5_array = new float[288];
	float *per25_array = new float[288];
	float *per50_array = new float[288];
	float *per75_array = new float[288];
	float *per95_array = new float[288];

	gen_agp(bg_nm_array, code_array, num_nm, per5_array, per25_array, per50_array, per75_array, per95_array);//agp

	//agp滤波
	float *per5_filtered_array = new float[288];
	float *per25_filtered_array = new float[288];
	float *per50_filtered_array = new float[288];
	float *per75_filtered_array = new float[288];
	float *per95_filtered_array = new float[288];

	per5_filtered_array = agp_filter(per5_array, 288);
	per25_filtered_array = agp_filter(per25_array, 288);
	per50_filtered_array = agp_filter(per50_array, 288);
	per75_filtered_array = agp_filter(per75_array, 288);
	per95_filtered_array = agp_filter(per95_array, 288);

	float mg_all = calc_mean(bg_nm_array, num_nm);
	float sdbg_all = calc_std(bg_nm_array, num_nm);
	float ea1c_all = mg2eA1c(mg_all);
	float gmi_all = mg2gmi(mg_all);
	float cv_all = calc_cv(sdbg_all, mg_all);
	float AUCmd = calc_AUCmd(per50_filtered_array);//AUCmd

	out << "\n";
	out << "mg_all: " << mg_all << "\n";
	out << "eA1c_all: " << ea1c_all << "\n";
	out << "gmi_all: " << gmi_all << "\n";
	out << "sdbg_all: " << sdbg_all << "\n";
	out << "cv_all: " << cv_all << "\n";
	out << "AUCmd: " << AUCmd << "\n";
	out << "\n";

	out << "num" << "\t" << "per5" << "\t" << "per25" << "\t" << "per50" << "\t" << "per75" << "\t" << "per95" << "\n";
	for (int i = 0; i < 288; i++)
	{
		out << i << "\t" << per5_filtered_array[i] << "\t" << per25_filtered_array[i] << "\t" << per50_filtered_array[i] << "\t" << per75_filtered_array[i] << "\t" << per95_filtered_array[i] << "\n";
	}

	delete[] per95_filtered_array;
	delete[] per75_filtered_array;
	delete[] per50_filtered_array;
	delete[] per25_filtered_array;
	delete[] per5_filtered_array;
	delete[] per95_array;
	delete[] per75_array;
	delete[] per50_array;
	delete[] per25_array;
	delete[] per5_array;
	delete[] bg_group_array;
	delete[] code_group_array;
	delete[] datetime_group_array;
	delete[] day_len_array;
	delete[] tir_code_array;
	delete[] correct_datetime_array;
	delete[] code_array;
	delete[] origin_datetime_nm_array;
	delete[] idx_array;
	delete[] bg_array;

	out.close();
	return 0;
}



