%%BEGIN(test)
%%(test)
%%(test[0])
%%(test[1])
%%(test[2])
%%END(test)
%%INCLUDE(test)
%%INCLUDE(test(par11))
%%INCLUDE(test(par21, par22))
%%BEGIN(test2)
>%%(test2)<
%%END(test2)
%%SET test2(1)
%%INCLUDE(test2)
%%INCLUDE(test2())
%%INCLUDE(test2(2))