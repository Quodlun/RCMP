#ifndef FINGERPRINTCLASS_H
#define FINGERPRINTCLASS_H

struct ClassInfo
{
    int     seatNumber;    // 座號
    String  name;       // 學生姓名

    // 建構函數，初始化班級名稱、座號和姓名
    ClassInfo ( int seat, String studentName ) : seatNumber(seat), name(studentName) {}
};

ClassInfo classArray [] =
{
    ClassInfo ( 8, "呂吉堂" ),
    ClassInfo ( 9, "曹銘洲" ),
    ClassInfo ( 11, "傅威禮" ),
    ClassInfo ( 12, "李佳諺" )
};

#endif