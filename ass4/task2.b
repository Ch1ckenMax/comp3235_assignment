for(i = 0; i < 8; i = i + 1;){
    for(j = 0; j < 8; j = j + 1;){
        if(i == 0 && j == 0){
            G[i,j].get(C);
            G[i,j].rotate(1);
        }
        else if(i == 0 && j == 7){
            G[i,j].get(C);
            G[i,j].rotate(2);
        }
        else if(i == 7 && j == 0){
            G[i,j].get(C);
            G[i,j].rotate(0);
        }
        else if(i == 7 && j == 7){
            G[i,j].get(C);
            G[i,j].rotate(3);
        }
        else if(i == 0){
            G[i,j].get(B);
            G[i,j].rotate(1);
        }
        else if(i == 7){
            G[i,j].get(B);
            G[i,j].rotate(3);
        }
        else if(j == 0){
            G[i,j].get(B);
        }
        else if(j == 7){
            G[i,j].get(B);
            G[i,j].rotate(2);
        }
        else{
            G[i,j].get(A);
        }
    }
}