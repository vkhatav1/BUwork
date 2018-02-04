
int myStrStr (char  haystack[], char needle[], char buffer[])
{
    int i,j,k,flag=0;

    // initialize the counters to zero
    i=0;
    j=0;
    k=0;
    while(i<strlen(haystack)) //continue the loop until the length of haystack
    {
        if(haystack[i]!=needle[j]) // if no match continue to next letter
        {
            i++;
        }
        else if(haystack[i]==needle[j]) // if match found, start other instance to check the next words from the current word
        {
            while(j<strlen(needle)) //continue the loop until the length of needle
            {
                if(haystack[i]==needle[j]) //if match found copy the letter in buffer from haystack and flag is set
                {
                    flag=1;
                    buffer[k]=haystack[i];
                    i++;
                    j++;
                    k++;
                }
                else
                {
                    flag=0;
                    break;
                }

            }
        }
    }

    return flag;
}
