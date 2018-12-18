
unsigned long gps_time, age, date;
long lat, lon;

String GPS_data(){
  while(Uart_gps.available() > 0){
    int c = Uart_gps.read();
    if(gps.encode(c))
      ;
  }
  gps.get_position(&lat, &lon, &age);
  gps.get_datetime(&date,&gps_time, &age);
  
  return "G"+String(lat)+";"+String(lon)+";"+String(gps_time);
}
