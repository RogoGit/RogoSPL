BEGIN { FS="[ :]" } 
{
   
    # preparing all necessary data	

    if ($2=="Savage") {
      	first_task[i1] = sprintf("%s %s: %s %s", $1, $2, $3, $4)
      	i1 = i1 + 1
    }

    if ($1=="Chet") {
    	second_task = sprintf("$%s, $%s, $%s", $5, $6, $7)
    }

    if ($5=="250") {
    	third_task[i2] = sprintf("%s %s", $1, $2)
	i2 = i2 + 1
    }

    amount = amount+1
    st = st + $5
    nd = nd + $6
    rd = rd + $7

}
END {
	print ("Пункт 1")
	for (j in first_task) { print (first_task[j]) }

	print ("\nПункт 2")
	print (second_task)

	print ("\nПункт 3")
	for (j in third_task) { print (third_task[j]) }

	print ("\nПункт 4")
	printf("%7s %10s %10s\n", "Месяц 1", "Месяц 2", "Месяц 3")
	printf("%7d %10d %10d\n", st, nd, rd)
	
	print ("\nПункт 5")
	printf("%7s %10s %10s\n", "Месяц 1", "Месяц 2", "Месяц 3")
	printf ("%7.2f %10.2f %10.2f\n", st/amount, nd/amount, rd/amount)

	print("\nПункт 6")
	#print strftime("Time = %m/%d/%Y %H:%M:%S", systime())
	system("date")
	system("ls")
}

