%%--
%%-- K�r detta med imp -i
%%--
%%BEGIN(foo(data))
`%%SET fum ("hej")
%%(data)
%%END(foo)
%%--
%%-- I raden nedan funkar det inte att "eskejpa bort en insert":
%%INCLUDE (foo ("`%%(fum)"))
%%--
%%-- men i n�sta rad funkar det:
hemskt mycket `%%(fum)
%%-- och i detta fall ska det inte fungera enligt manualen, �ven om imp/75
%%-- antyder att det vore bra...
%%INCLUDE (foo ("%`%(fum)"))
%%--
%%-- Jag vill allts� egentligen producera f�ljande (f�rutom kommentarstecknena):
%%-- %%SET fum ("hej")
%%-- %%(fum)
%%-- hemskt mycket %%(fum)
