#dependencies
import socket 
import time 
import mysql.connector


#mysql database connector
mydb = mysql.connector.connect(
  host = "localhost",
  user = "root", 
  password = "@Godown69",
  database = "NEW_ESCORTS"  
)

mycursor =mydb.cursor()
# mycursor.execute("CREATE DATABASE NEW_ESCORTS")  //to create new databases

# to create tables

# mycursor.execute("CREATE TABLE GYRO(time long, gyroX float, gyroY float, gyroZ float) ");

# mycursor.execute("CREATE TABLE VIBRATION(time long, Hz long)");

# mycursor.execute("CREATE TABLE TEMPERATURE(time long ,Celcius float)");

s=socket.socket()
s.bind(('0.0.0.0',5767))
s.listen(8)


while True:
    Time = round(time.time() * 1000) #timeseries in milliseconds
    mycursor =mydb.cursor()
    c,addr = s.accept()
    
    data = c.recv(1024).decode("utf-8") #receiving and decoding data 
    DATA = data.split()
    print(DATA)
    
    
    #inserting into first table
    q1 = "INSERT INTO TEMPERATURE(time,Celcius) VALUES(%s,%s)"
    val1 = (Time ,DATA[0])
    mycursor.execute(q1,val1)
    
     #inserting into second table
    q2 = "INSERT INTO VIBRATION(time,Hz) VALUES(%s,%s)"
    val2= (Time,DATA[1])
    mycursor.execute(q2,val2)
    
     #inserting into third table
    q3 = "INSERT INTO GYRO(time, gyroX, gyroY, gyroZ) VALUES(%s,%s,%s,%s)"
    val3= (Time,DATA[5],DATA[6],DATA[7])
    mycursor.execute(q3,val3)
    
    #commiting all the above insertion
    mydb.commit()
    
    #delay in code for better and accyrate database
    time.sleep(1)
    
    print("RECORD INSRTED")
  
