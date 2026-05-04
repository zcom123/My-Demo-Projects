#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<graphics.h>
#include<time.h>


int main() {
	initgraph(800, 600,1);//打开黑色窗口
	srand((unsigned)time(NULL));//生成种子

	float price = 10.0f;
	float change;

	float history[100] = { 0 };
	int count = 0;
	int pause = 0;

	float money = 10000.0f;
	int hold = 0;
	float cost = 0.0f;
	float profit = 0.0f;

	float volatility = 0.3f;//波动幅度
	float trend = 0.0f;//趋势方向
	int trend_time = 0;//趋势持续步数

	while (1) {
		if (GetAsyncKeyState(VK_SPACE) & 0x8000) { // 按下空格键暂停/继续
			pause = !pause;
			Sleep(200); // 防止按键抖动
		}

		if (GetAsyncKeyState('B') & 0x8000) { // 按下B键买入
			if (money >= price*100) {
				hold+=100;
				money -= price * 100;
				cost = price;
				Sleep(200);
			}
		}

		if (GetAsyncKeyState('S') & 0x8000) {
			if (hold > 0) {
				money += hold * price;
				profit += (price - cost) * hold;
				hold = 0;
				Sleep(200);
			}
		}

		if (!pause) {
			if (trend_time <= 0) {
				trend_time = rand() % 13 + 3;	// 持续 3~15 步
				trend = (rand() % 61 - 30) / 100.0f; // -0.3 ~ +0.3 趋势
				volatility = (rand() % 130 + 20) / 100.0f; // 波动大小

				// 5%概率触发极端行情（大波动）
				if (rand() % 100 < 5) {
					volatility *= 2.5f;
				}
			}
			else {
				trend_time--;
			}

			// 最终涨跌 = 波动 + 趋势（真实走势核心）
			float rnd = (rand() % 1001 - 500) / 1000.0f;
			change = volatility * rnd + trend;

			price += change;

			if (price < 3)price = 3;
			if (price > 30)price = 30;

			history[count] = price;
			count++;
			if (count >= 100)count = 0;
		}
		cleardevice();

		//画出来K线图
		for (int i = 0; i < count - 1; i++) {
			int x1 = i * 8;
			int y1 = 600 - history[i] * 20;
			int x2 = (i + 1) * 8;;
			int y2 = 600 - history[i + 1] * 20;
			
			if (history[i + 1] > history[i]) {
				setcolor(RED);   // 涨：红色
			}
			else {
				setcolor(GREEN); // 跌：绿色
			}

			line(x1, y1, x2, y2);
		}

		setcolor(WHITE);   // 文字用白色

		// 1. 当前股价
		char str_price[50];
		sprintf(str_price, "Price: %.2f", price);
		outtextxy(10, 10, str_price);

		// 2. 资金
		char str_money[50];
		sprintf(str_money, "Money: %.2f", money);
		outtextxy(10, 30, str_money);

		// 3. 持仓
		char str_hold[50];
		sprintf(str_hold, "Hold: %d", hold);
		outtextxy(10, 50, str_hold);

		// 4. 持仓盈亏
		char str_now_profit[50];
		sprintf(str_now_profit, "Profit: %.2f", hold * (price - cost));
		outtextxy(10, 70, str_now_profit);

		// 5. 总盈亏
		char str_total_profit[50];
		sprintf(str_total_profit, "Total Profit: %.2f", profit);
		outtextxy(10, 90, str_total_profit);

		// 6. 暂停/运行状态
		char str_status[50];
		if (pause)
			sprintf(str_status, "Status: PAUSED");
		else
			sprintf(str_status, "Status: RUNNING");
		outtextxy(10, 110, str_status);

		system("cls");   // 清屏
		printf("当前股价：%.2f 元\n", price);
		printf("------------------------\n");
		printf("持仓：%d 股\n", hold);
		printf("资金：%.2f 元\n", money);
		printf("盈亏：%.2f 元\n", profit);
		printf("------------------------\n");
		if (pause)
			printf("状态：已暂停\n");
		else
			printf("状态：运行中\n");
		float now_profit = hold * (price - cost);
		printf("持仓盈亏：%.2f 元\n", now_profit);
		Sleep(100);
	}

	closegraph();//关闭窗口
	return 0;
}