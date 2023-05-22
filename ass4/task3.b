//Pattern: Funny Wave Haha Yay

//Template blocks to be copied
G[0,0].get(C);
G[0,0].rotate(2);
G[0,1].get(C);

//Use a while-loop to iterate through each coordinate
i = 0;
while (i < 8){
    j = 0;
    while(j < 8){
        if( ((i + j) % 2) == 0){
            G[i,j] = G[0,0];
        }
        else{
            G[i,j] = G[0,1];
        }
        j = j + 1;
    }
    i = i + 1;
}