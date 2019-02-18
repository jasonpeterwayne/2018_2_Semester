`timescale 1ns / 1ps

module fulladder(input A, input B, input CIN, output Sum, output Cout);
    
    //asign Sum=(A&B&CIN)|(~A&B&~CIN)|(~A&~B&CIN)|(A&~B&~CIN);
    wire AN, BN, CINN;
    wire y0, y1, y2, y3;
    //asign Cout=(A&B)|(B&CIN)|(CIN&A);
    wire z0, z1, z2;
    
    not(AN, A);
    not(BN, B);
    not(CINN, CIN);
    
    and(y0, A, B, CIN);
    and(y1, AN, B, CINN);
    and(y2, AN, BN, CIN);
    and(y3, A, BN, CINN);
    or(Sum, y0, y1, y2, y3);
    
    and(z0, A, B);
    and(z1, B, CIN);
    and(z2, A, CIN);
    or(Cout, z0, z1, z2);
      
endmodule
