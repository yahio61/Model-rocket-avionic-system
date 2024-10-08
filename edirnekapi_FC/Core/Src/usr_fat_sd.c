/*
 * usr_fat_sd.c
 *
 *  Created on: 28 Ağu 2024
 *      Author: numan
 */

#include "usr_fat_sd.h"
#include "strain_gauge.h"

#ifdef GNSS_SD_INTEGRATION_MODE
_io float lat = 0;
_io float lon = 0;
_io float kn = 0;
_io float time = 0;
_io int fixQuality = 0;
_io int satInUse = 0;
_io int hdop = 0;
_io float altitude_M = 0;
_io int test_cnt = 0;
_io float test_fl = 0.2;
#endif
_io float testNum = 0;
_io char fatBuff[1024];
char _aBuf[2048];
_io char _line[] = "\n-----------------------------------\n";


extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef huart3;
extern hx711_t loadcell;

_io FATFS fs;
_io FIL fil;
_io FRESULT fresult;
_io UINT br, bw;
_io FATFS *pfs;
_io DWORD fre_clust;
_io uint32_t total, free_space;
/*
static BME_280_t BME_Pack;
bmi088_struct_t BMI_Pack;
//static lorastruct Backup_Pack;
static S_GPS_L86_DATA GPS_Pack;
static power Power_Pack;
backup_sram_datas_s *Backup_Pack = (backup_sram_datas_s *)BKPSRAM_BASE;
*/

_io void uart_Sd_Transmit(char *string)
{
	uint8_t len = strlen(string);
	HAL_UART_Transmit(&huart3, (uint8_t *)string, len, 2000); // transmit in blocking mode
}
_io int sdBufSize(char *buf)
{
	int i = 0;
	while (*buf++ != '\0')
		i++;
	return i;
}
_io void sdBufClear(void)
{
	for (int i = 0; i < 1024; i++)
		fatBuff[i] = 0;
}
/* //!Deprecated
void transmitter(_TEST_PAYLOAD1 *p1Data, _TEST_PAYLOAD2 *p2Data)
{
	fresult = f_open(&fil, "testDataLogFile.txt", FA_CREATE_ALWAYS | FA_WRITE);
	// testNum = p1Data->a;
	char aBuf[1024];
	sprintf(aBuf, "a: %.2f, b: %.2f, c: %.2f , speed: %.3f, time: %f\n", p1Data->a, p1Data->b, p1Data->c, p2Data->speedKN, p2Data->timeDateBuf);
	fresult = f_printf(&fil, aBuf);
	fresult = f_close(&fil); // close
}*/
_io void listFiles()
{
	DIR dir;	 // Dizin işaretçisi
	FILINFO fno; // Dosya bilgilerini tutacak yapı
	// FRESULT res; // İşlem sonucu

	/*// Dizin açma
	res = f_opendir(&dir, path);
	if (res != FR_OK)
	{
		printf("Dizin açılamadı: %d\n", res);
		return;
	}

	// Dizin içeriğini oku
	while (1)
	{
		res = f_readdir(&dir, &fno); // Bir sonraki dosyayı oku
		if (res != FR_OK || fno.fname[0] == 0)
		{
			break; // Okuma hatası veya dizin sonu
		}

		// Dosya adı yazdır
		printf("Dosya adı: %s\n", fno.fname);
	}

	// Dizin kapat
	f_closedir(&dir);*/
	f_opendir(&dir, "/"); // Open Root
	do
	{
		f_readdir(&dir, &fno);
		if (fno.fname[0] != 0)
		{
			// printf("File found: %s\n", fno.fname); // Print File Name
			uart_Sd_Transmit("\n File found: %s");
			uart_Sd_Transmit(fno.fname);
		}
	} while (fno.fname[0] != 0);

	f_closedir(&dir);
}
PUBLIC void usr_fatfsInitial(void)
{
	MX_FATFS_Init();
}
PUBLIC void sdInitials(void)
{
	fresult = f_mount(&fs, "/", 1);
	if (fresult != FR_OK)
		uart_Sd_Transmit("error in mount [Check For Fat Response (fresult) in debug mode!!!!]......\n");
	else
		uart_Sd_Transmit("SD CARD MOUNTED CUSSESSFULY..\n");

	/*capacity details*/
	f_getfree("", &fre_clust, &pfs);
	total = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
	sprintf(fatBuff, "SD CARD [Total] Size: \t%lu\n", total);
	uart_Sd_Transmit(fatBuff);
	sdBufClear();
	free_space = (uint32_t)(fre_clust * pfs->csize * 0.5);
	sprintf(fatBuff, "SD CARD [Free] Size: \t%lu\n", free_space);
	uart_Sd_Transmit(fatBuff);
	fresult = f_open(&fil, "flightRecorder.csv", FA_CREATE_NEW | FA_WRITE);//test
	fresult = f_close(&fil);
}

PUBLIC void openFolder(void)
{
	/*fresult = f_open(&fil, "testDataLogFile.txt", FA_CREATE_ALWAYS | FA_WRITE); // open to write

	fresult = f_puts("\n--------[GOK-HAN Data Logger]------\n\n", &fil); // write

	fresult = f_close(&fil); // close
	uart_Sd_Transmit("[testDataLogFile.txt] created, GOK-HAN data has been written.\n");

	fresult = f_open(&fil, "testDataLogFile.txt", FA_READ); // open to read
	f_gets(fatBuff, fil.fsize, &fil);                       // read string
	uart_Sd_Transmit(fatBuff);
	f_close(&fil);
	sdBufClear();*/
}
PUBLIC void usrFatTest(void)
{
#ifdef GNSS_SD_INTEGRATION_MODE
	char m_DataBuf[200];
	fresult = f_open(&fil, "testDataLogFile.txt", FA_CREATE_ALWAYS | FA_WRITE);
	fresult = f_puts("\n--------[L86 Data Values]------\n", &fil);
	while (1)
	{
		Usr_GpsL86GetValues(&L86DataParams);
		lat = L86DataParams.lat;
		lon = L86DataParams.lon;
		kn = L86DataParams.speedKN;
		time = L86DataParams.timeDateBuf;

		if (lon > 20)
		{
			// sprintf(m_DataBuf, "counter: %d, float: %f\n", test_cnt, test_fl);
			sprintf(m_DataBuf, "Lat: [%f] Lon: [%f] Speed: [%.2f] Time: [%f]\n", lat, lon, kn, time);
			fresult = f_printf(&fil, m_DataBuf);
			test_cnt++;
			if (test_cnt > 100)
			{
				HAL_Delay(1000);
				uart_Sd_Transmit("[testDataLogFile.txt] GOK-HAN data has been modified.\n");
				fresult = f_close(&fil); // close
				uart_Sd_Transmit("[testDataLogFile.txt] GOK-HAN data log closed.\n");
				break;
			}
		}
	}
#endif

#ifdef FAT_TEST_MODE
	char aBuf[50];
	fresult = f_open(&fil, "testDataLogFile.txt", FA_CREATE_ALWAYS | FA_WRITE); // open to write
	fresult = f_puts("\n--------[Counter Started]------\n", &fil);
	fresult = f_puts("\n--------[saaaa]------\n", &fil);
	while (1)
	{
		// fresult = f_printf(&fil, "sa %d\n",test_cnt);
		sprintf(aBuf, "counter: %d, float: %f\n", test_cnt, test_fl);
		fresult = f_printf(&fil, aBuf);
		test_cnt++;
		test_fl++;
		HAL_Delay(100);
		if (test_cnt > 40)
		{
			uart_Sd_Transmit("[testDataLogFile.txt] GOK-HAN data has been modified.\n");
			break;
		}
	}
	fresult = f_close(&fil); // close
	uart_Sd_Transmit("[testDataLogFile.txt] GOK-HAN data log closed.\n");

#endif
}

int sd_csv_log_transmit(const char *str)
{
	fresult = f_open(&fil, "flightRecorder.csv", FA_OPEN_APPEND | FA_WRITE);
	sprintf(_aBuf,"--;;; %s ;;;--\n",str);
	fresult = f_printf(&fil, _aBuf);
	fresult = f_close(&fil);
	for (int i = 0; i < 2024; i++)
		_aBuf[i] = 0;
	return fresult;
}

PUBLIC int sd_transmit(const char *str)
{
	uint8_t _buffer[250];
#ifdef ROCKET_CARD
	sprintf((char*)_buffer, "Counter;Time;Pressure(mPa);Humidity(%%);Temperature(C);Altitude(m);Velocity(m/s);Acc_X;Acc_Y;Acc_Z;Gyro_X;Gyro_Y;Gyro_Z;Rocket_Status;Angle(degree);Q0;Q1;Q2;Q3;Max_altitude(m);Latitude;Longitude;Sat_count;GPS_altitude;Voltage;Current(mA);Power(mWs)\n");
#else
	sprintf((char*)_buffer, "Counter;Time;Pressure(mPa);Humidity(%%);Temperature(C);Altitude(m);Velocity(m/s);Acc_X;Acc_Y;Acc_Z;Gyro_X;Gyro_Y;Gyro_Z;Rocket_Status;Angle(degree);Q0;Q1;Q2;Q3;Strain;Latitude;Longitude;Sat_count;GPS_altitude;Voltage;Current(mA);Power(mWs)\n");
#endif
	fresult = f_open(&fil, "flightRecorder.csv", FA_OPEN_APPEND | FA_WRITE); // open to write
	fresult = f_printf(&fil, (char*)_buffer);
	fresult = f_close(&fil); // close
	return fresult;
}
PUBLIC void sdDataLogger(uint32_t counter, BME_280_t *BME_Pack, bmi088_struct_t *BMI_Pack, backup_sram_datas_s *Backup_Pack, S_GPS_L86_DATA *GPS_Pack, power *Power_Pack)
{
	fresult = f_open(&fil, "flightRecorder.csv", FA_OPEN_APPEND | FA_WRITE);
#ifdef ROCKET_CARD
	sprintf(_aBuf, "%lu;%.0f;%.3f;%.1f;%.1f;%.1f;%.1f;%f;%f;%f;%f;%f;%f;%d;%.2f;%f;%f;%f;%f;%.1f;%f;%f;%d;%.1f;%.2f;%.2f;%.2f\n", counter, GPS_Pack->timeDateBuf, BME_Pack->pressure, BME_Pack->humidity, BME_Pack->temperature, BME_Pack->altitude, BME_Pack->velocity, BMI_Pack->acc_x, BMI_Pack->acc_y, BMI_Pack->acc_z, BMI_Pack->gyro_x, BMI_Pack->gyro_y, BMI_Pack->gyro_z, Backup_Pack->r_status, BMI_Pack->angle, Backup_Pack->q[0], Backup_Pack->q[1], Backup_Pack->q[2], Backup_Pack->q[3], Backup_Pack->max_altitude, GPS_Pack->lat, GPS_Pack->lon, GPS_Pack->satInUse, GPS_Pack->altitudeInMeter, Power_Pack->voltaj, Power_Pack->akim, Power_Pack->mWatt_s);
#else
	sprintf(_aBuf, "%lu;%.0f;%.3f;%.1f;%.1f;%.1f;%.1f;%f;%f;%f;%f;%f;%f;%d;%.2f;%f;%f;%f;%f;%.1f;%f;%f;%d;%.1f;%.2f;%.2f;%.2f\n", counter, GPS_Pack->timeDateBuf, BME_Pack->pressure, BME_Pack->humidity, BME_Pack->temperature, BME_Pack->altitude, BME_Pack->velocity, BMI_Pack->acc_x, BMI_Pack->acc_y, BMI_Pack->acc_z, BMI_Pack->gyro_x, BMI_Pack->gyro_y, BMI_Pack->gyro_z, Backup_Pack->r_status, BMI_Pack->angle, Backup_Pack->q[0], Backup_Pack->q[1], Backup_Pack->q[2], Backup_Pack->q[3], strain_gage_get_vals(&loadcell), GPS_Pack->lat, GPS_Pack->lon, GPS_Pack->satInUse, GPS_Pack->altitudeInMeter, Power_Pack->voltaj, Power_Pack->akim, Power_Pack->mWatt_s);
#endif
	fresult = f_printf(&fil, _aBuf);
	fresult = f_close(&fil);// SD CLOSE
	for(int i =0;i<2024;i++)
		_aBuf[i] = 0 ;
}
