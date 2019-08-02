# *.csv to *.sql Convertor
## 📢 개요
 csv(comma-separated variables) 파일은 각각의 값들이 콤마(,;comma)로 분리되어있는 텍스트 파일이다. 오래전부터 스프레드시트나 DB에서 많이 쓰여왔다. 구현이 간단하며 호환되지 않는 포맷을 사용하는 프로그램 간의 자료 전달에 사용한다. 
 

|  <center>연도</center> |  <center>제조사</center> |  <center>모델</center> |  <center>설명</center> |  <center>가격</center> |
|:--------|:--------|:--------|:--------|:--------|
|1997 |Ford |E350 |ac, abs, moon |3000.00|
|1999 |Chevy |Venture "Extended Edition" | |4900.00|
|1999 |Chevy |Venture "Extended Edition, Very Large" | |5000.00|
|1996 |Jeep |Grand Cherokee |MUST SELL! <br> air, moon roof, loaded |4799.00|

 스프레드시트에서는 위와 같이 표시되는 테이블을 csv 파일로 변환하면 다음과 같이 표현된다.
 
 ```text
    연도,제조서,모델,설명,가격
    1997,Ford,E350,"ac, abs, moon",3000.00
    1999,Chevy,"Venture ""Extended Edition""","",4900.00
    1999,Chevy,"Venture ""Extended Edition, Very Large""",,5000.00
    1996,Jeep,Grand Cherokee,"MUST SELL!
    air, moon roof, loaded",4799.00
 ```
 
 데이터 관리 측면에서도 유리하다. 각종 데이터를 하드코딩으로 프로그램에 박기 보다는 스프레드시트와 같은 외부 파일에 데이터들을 저장하고 관리하는 편이 여러모로 유연성이 높기 때문에 많은 분야에서 사용된다.
 
   ![capture](https://github.com/kbm0996/-Utility-csv2sql_Convertor/blob/master/csv2sql_Convertor/jpg/ItemExtendedCost.PNG)
  
  **figure 1. ItemExtendedCost*
 
 

## 💻 csv2sql_Convertor

  ![capture](https://github.com/kbm0996/-Utility-csv2sql_Convertor/blob/master/csv2sql_Convertor/jpg/accounttable_xls.png)
  
  **figure 2. RawData*

   ![capture](https://github.com/kbm0996/-Utility-csv2sql_Convertor/blob/master/csv2sql_Convertor/jpg/run.png)
  
  **figure 3. Convertor Run*
 
    ![capture](https://github.com/kbm0996/-Utility-csv2sql_Convertor/blob/master/csv2sql_Convertor/jpg/accounttable_test_sql.PNG)
  
  **figure 3. ResultData*
 
