#==============================================================================
#
# Bruce's Income/Outgo calculator
#
# (Invaluable in the job search)
#

import sys

# Cities and miles one way...
cities = {"b":(19.2,"Basingstoke",["bas","basingstoke"]),
          "r":(0,   "Reading",    ["rea","read","reading"]),
          "o":(42,  "Oxford",     ["ox","oxf","oxford"]),
          "m":(15,  "Maidenhead", ["maid","maidenhead"]),
          "t":(6,   "Theale",     ["th","theale"]),
         }
allcities = {}

# Outgo Per Week...
outPerWeek      = {
                  #"van":183,
                  "rent":114.36,
                  "council":(15.80+9),
               }
               
# Outgo Per Fortnight...
outPerFortnight = {
                  "elec":10,
                  "water":5,
               }

# Outgo Per Month...
outPerMonth     = {
                  "insur":69.0,
                  "phone":29.0,
                  #"rent":665.0,
                  "cartax":20.0,
                  #"council":120,
               }
               
# Income Per Month...
standardLife = 52 # Existing, already taxed, monthly Standard Life income

# Income Per day ...
#vandriving = 128.0

# Driving var...
workingWeeksPerYear = 48

# temp...
petrol = 0

def perweek(income): # Given annual income, calculate per week money in pocket...

  i = income - max(0,income - 11000)*.2 - max(0,income - 11000)*.12 + standardLife*12
  weeklyIncome = i/52.0
  petrol = 0.0
  
  weeklyOutGo = petrol
  for key in outPerWeek:
    weeklyOutGo += outPerWeek[key]
  for key in outPerFortnight:
    weeklyOutGo += outPerFortnight[key]/2.
  for key in outPerMonth:
    weeklyOutGo += outPerMonth[key]*12.0/52.0
    
  print("Income/week: %d"%int(weeklyIncome))
  print("Income/year: %d"%int(weeklyIncome*52.0))
  print("Outgo/week:  %d"%int(weeklyOutGo))
  print("")
  
  return int(weeklyIncome-weeklyOutGo)

def askexit():

  yn = 'yn'
  while True:
    print('')
    input = raw_input("Exit (Y/N)? ").strip().lower()
    if input not in yn:
      print('')
      print("Hey, I said \"enter 'Y' or 'N'\" kiddo")
      continue
    ans = True if input == 'y' else False
    if not ans:
      print('')
    return ans
    
if __name__ == '__main__':

  print('*** Weekly Income Calculator for Basingstoke ***')
  
  print('')
  
  while True:

    try:
      input = raw_input("Annual income? ").strip()
      if not input:
        yn = askexit()
        if not yn:
          continue
        sys.exit(0)
      income = int(input)
      pw = perweek(income)
      print('')
      print('Pounds per week left over: %5.2f'%pw)
      print('')
    except ValueError:
      print("***Please input an INTEGER value!!!")
  
    weekly = perweek(income)