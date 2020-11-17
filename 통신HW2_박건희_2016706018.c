//2016706018 �ڰ���
//header file
#include <stdio.h>
#include <math.h>	//����, �ڻ����Լ� ����� ���� ȣ��

#define PI 3.1415926535897	//���� ����

void Binary_PSK(char file_name[20], int phase, int sample_num, int bit_num);
// BPSK�� ���� bit����
// �����̸�, ��������, ���ð���, ��Ʈ��


void Quadrature_PSK(char file_name[20], int phase, int sample_num, int bit_num);
// QPSK�� ���� bit����
// �����̸�, �⺻����, ���ð���, ��Ʈ��


//main function
int main() {
	Binary_PSK("bpsk1.dat", 180, 20, 1000);	//bpsk1.dat, symbol�� phase���� 180, ��Ʈ�� ���ü� 20, ��Ʈ�� 1000
	Binary_PSK("bpsk2.dat", 90, 20, 1000);		//bpsk2.dat, symbol�� phase���� 90, ��Ʈ�� ���ü� 20, ��Ʈ�� 1000
	Binary_PSK("bpsk3.dat", 180, 10, 1000);	//bpsk3.dat, symbol�� phase���� 180, ��Ʈ�� ���ü� 10, ��Ʈ�� 1000
	Quadrature_PSK("qpsk.dat", 45, 40, 1000);		//qpsk.dat, symbol phase 45������ 90���� ȸ��, ��Ʈ�� ���ü� 40, ��Ʈ�� 1000		
}



// BPSK function
void Binary_PSK(char file_name[20], int phase, int sample_num, int bit_num) {
	
	FILE* fin, * fbit;						//���ϱ���ü�� ������ ����
	fopen_s(&fin, file_name, "rt");			//fin�� file_name ���ڿ� ���� ���� txt ���·� �б�
	fopen_s(&fbit, "bit.dat", "rt");		//fbit�� bit.dat ���� txt ���·� �б�
	float input[20] = { 0.0, };				// ���� �迭, sample ���� 10�� �Ǵ� 20���� ���� ����ؼ� 20���� ����
	float sum1 = 0, sum2 = 0;				// ML Rule�� �ǰ��� ���а�
	int detect;								// ������ bit
	int bit;								// bit.dat�� ����� ���� bit
	int i, k;								// �ݺ�����
	float error_count = 0;					// ���� bit ����

	for (i = 0; i < bit_num; i++) {			//bit���� ��ŭ �ݺ�
		for (k = 0; k < sample_num; k++) {	//bit�� sample����ŭ �ݺ�
			fscanf_s(fin, "%f", &input[k]);	//bit�� sample�� input�迭�� ������� ����
			sum1 += (input[k] * cos((2*PI*k)/10));									//sample�� �ڻ���(���� 0��)�� correlation
			sum2 += (input[k] * cos(((2*PI*k)/10) + (phase*PI/180)));				//sample�� �ڻ���(�Է¹��� ����)�� correlation
			//������ ����� 180���ϋ��� -cos�� �����ϰ�, 90���϶��� -sin�� ������
		}									//for�� ����� sum1, sum2�� �� phase���� sample ���а� ����
		//���а��� ũ��� �� ����
		if (sum1 > sum2)						
			detect = 1;					
		else if (sum1 < sum2)
			detect = 0;
		fscanf_s(fbit, "%d", &bit);			//���� bit �о����
		if (detect != bit)					//���� ������ bit�� ���� bit�� �ٸ��� 
			error_count++;					//���� ���� �߰�

		sum1 = 0; sum2 = 0;							//���а� 0 �ʱ�ȭ
	}
	//��� bit�� ���� ����

	printf("%s Binary_PSK error_count: %d, error_rate: %.1f%%\n", file_name, (int)error_count, error_count / (float)bit_num * 100);		//���� �̸�, ������Ʈ����, ��Ʈ������ ���
}

// QPSK function
void Quadrature_PSK(char file_name[20], int phase, int sample_num, int bit_num) {

	FILE* fin, * fbit;						//���ϱ���ü�� ������ ����
	fopen_s(&fin, file_name, "rt");			//fin�� file_name ���ڿ� ���� ���� txt ���·� �б�
	fopen_s(&fbit, "bit.dat", "rt");		//fbit�� bit.dat ���� txt ���·� �б�
	float input[40] = { 0.0, };				// ���� �迭, sample ���� 40���� ���� ����ؼ� 40���� ����
	float sum1= 0, sum2 = 0;				// ML Rule�� �ǰ��� ���а�
	int detect;								// ������ bit
	int bit;								// bit.dat�� ����� ���� bit
	int i, k;								// �ݺ����� 
	float error_count = 0;					// ���� bit ����
	int sym_detect;
	for (i = 0; i < bit_num/2; i++) {			//symbol�� 2bit�� �����Ƿ� bit���� ���� ��ŭ �ݺ�
		for (k = 0; k < sample_num; k++) {	//bit�� sample����ŭ �ݺ�
			fscanf_s(fin, "%f", &input[k]);	//bit�� sample�� input�迭�� ������� ����
			sum1 += (input[k] * cos((2 * PI * k) / 10 + (phase / 180 * PI)));				//sample�� �ڻ��� 45������ ����
			sum2 += (input[k] * sin((2 * PI * k) / 10 + (phase / 180 * PI)));				//sample�� ���� 45������ ����

		}									//for�� ����� sum1,sum2�� bit �ϳ��� cos45��,sin45���� ���� sample ���а� ����

		fscanf_s(fbit, "%d", &bit);			//���� bit �о����
		if (sum1>0)							//MSB���� �ڻ���45�� ��ġ���� ���а��� ��ȣ������ ��Ʈ ����
			detect = 1;
		else detect = 0;

		if (detect != bit)					//���� ������ bit�� ���� bit�� �ٸ��� 
			error_count++;					//���� ���� �߰�
	
		fscanf_s(fbit, "%d", &bit);			//���� bit �о����
		if (sum2>0)							//LSB���� ����45�� ��ġ���� ���а��� ��ȣ������ ��Ʈ ����
			detect = 1;
		else detect = 0;
		
		if (detect != bit)					//���� ������ bit�� ���� bit�� �ٸ��� 
			error_count++;					//���� ���� �߰�

		sum1 = 0; sum2 = 0;
	}
	//��� bit�� ���� ����

	printf("%s Quadrature_PSK error_count: %d, error_rate: %.1f%%\n", file_name, (int)error_count, error_count / (float)bit_num * 100);		//���� �̸�, ������Ʈ����, ��Ʈ������ ���
}


