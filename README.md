This repo is for the projects assigned in cs 6740.

# How to Compile
I used CLion to compile this project, but you should be able to use gcc on file at the following path:
cs6740_submission/cmake-build-debug/cs6740

To run the program use the command ./cs6740 in the cmake-build-debug directory.

The program itself has prompts to navigate, and it should be self-explanatory.

The password is "admin1234"

# Employee Directory Logic
The program operates as a setuid executable, allowing it to run with root permissions when editing the password.txt,
rather than the user executing it. When a user runs the program with their own permissions the setuid bit grants them
elevated privileges. The program includes functions for managing an employee directory, requiring user authentication
to modify records. It securely prompts for passwords without showing the user's input on the screen and verifies the
passwords match before updating the password file. The permissions to the password file are only changed right before
and right after actually changing the password. This ensures that sensitive operations are executed securely while
maintaining the principle of least privilege.

# Crypto Cracker 

## Logic
The program accepts the name of a file and tries to detect if the text was encrypted using a Caesar cipher, a Vigenere 
cipher, a columnar cipher, or another unidentified cipher. To determine if the text was encrypted with a Caesar cipher
the program counts each letter and takes the most common and checks to see if it is more than 10% of th total. If that
letter is not 'e' then the program determines it is a Caesar cipher and uses that letter to determine the key. To
decrypted columnar cipher the program uses the same logic but determines it's a columnar cipher if the most common
letter is 'e'. The program then tries decrypting the text with the factors of the length of the text and checks to see
which decrypted text contains the most common English words. To decrypt Vigenere cipher the program performs a Kasiski
examination of the text and tries different key lengths up to a max of 12 with frequency analysis to determine the 
potential key length of the Vigenere cipher. If none of these are detected the program determines it is another cipher.

## Program Results
ciphertext1.txt
Detected Caesar cipher with key: k
Sample of decrypted text (Caesar):
IAMHAPPYTOJOINWITHYOUTODAYINWHATWILLGODOWNINHISTORYASTHEGREATESTDEMONSTRATIONFORFREEDOMINTHEHISTORYOFOURNATIONFIVESCOREYEARSAGOAGREATAMERICANINWHOSESYMBOLICSHADOWWESTANDTODAYSIGNEDTHEEMANCIPATIONPROCL...
Time taken to calculate result 0.006 seconds

ciphertext2.txt
Detected Columnar cipher with key 12
Sample of decrypted text (Columnar):
WETHEPEOPLEOFTHEUNITEDSTATESINORDERTOFORMAMOREPERFECTUNIONESTABLISHJUSTICEINSUREDOMESTICTRANQUILITYPROVIDEFORTHECOMMONDEFENCEPROMOTETHEGENERALWELFAREANDSECURETHEBLESSINGSOFLIBERTYTOOURSELVESANDOURPOST...
Time taken to calculate result 0.051 seconds

ciphertext3.txt
Detected other cipher
Time taken to calculate result 0.002 seconds

ciphertext4.txt
Detected Vigenere cipher with key: loganutah
Sample of decrypted text (Vigenere):
CALLMEISHMAELSOMEYEARSAGONEVERMINDHOWLONGPRECISELYHAVINGLITTLEORNOMONEYINMYPURSEANDNOTHINGPARTICULARTOINTERESTMEONSHOREITHOUGHTIWOULDSAILABOUTALITTLEANDSEETHEWATERYPARTOFTHEWORLDITISAWAYIHAVEOFDRIVING...
Time taken to calculate result 0.007 seconds

ciphertext5.txt
Detected Columnar cipher with key 6
Sample of decrypted text (Columnar):
YOUWILLREJOICETOHEARTHATNODISASTERHASACCOMPANIEDTHECOMMENCEMENTOFANENTERPRISEWHICHYOUHAVEREGARDEDWITHSUCHEVILFOREBODINGSIARRIVEDHEREYESTERDAYANDMYFIRSTTASKISTOASSUREMYDEARSISTEROFMYWELFAREANDINCREASIN...
Time taken to calculate result 0.030 seconds

ciphertext6.txt
Detected Vigenere cipher with key: we
Sample of decrypted text (Vigenere):
HOWSLOWLYTHETIMEPASSESHEREENCOMPASSEDASIAMBYFROSTANDSNOWYETASECONDSTEPISTAKENTOWARDSMYENTERPRISEIHAVEHIREDAVESSELANDAMOCCUPIEDINCOLLECTINGMYSAILORSTHOSEWHOMIHAVEALREADYENGAGEDAPPEARTOBEMENONWHOMICANDE...
Time taken to calculate result 0.006 seconds