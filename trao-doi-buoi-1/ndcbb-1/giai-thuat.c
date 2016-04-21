float giaithuat(){ //Giai thuat loc nhieu, cai tien so lieu do tu cam bien
	/*Giai thuat loc nhieu:
	 * Input: Mang gia tri A co n phan tu
	 * Processing:
	 * Thuc hien vong lap theo sai so: 50% 40% 30% 20% 10% 5%
	 * Lap theo tung gia tri A[i]
	 * + Tinh trung binh bo A[i]
	 * + So sanh: Neu |trung_binh_bo_A[i]-Ai| > trunh_binh_bo_A[i]*sai_so =====> Loai bo A[i] ra khoi tap dang xet
	 * + Giu lai cac phan tu con dang duoc xet va thuc hien tiep vong lap
	 * Output: Gia tri trung binh cua cac phan tu con duoc giu lai
	 */
	int i,j,k,t,m,kt=0;
	float average;

	for (i=0;i<N_error;i++){//Lap theo tung gia tri sai so
		if (N>=2){ //Mang 2 phai tu 2 phan tu tro len moi xet tiep
			for (j=0;j<N;j++){//Lap theo tung gia tri khoang cach
				float s=0;
				for (k=0;k<N;k++){
					if (j != k){//Khong tinh phan tu A[j] dang xet
						s=s+A[k];
					}
				}
				s=s/(N-1);//Gia tri trung binh khong tinh A[j];
				if (abs(s-A[j])>(s*error_epsilon[i])){
					kt=1;
					m=0;
					for (k=0;k<N;k++){
						if (k != j){
							A_tmp[m]=A[k];
							m++;
						}
					}
					for (k=0;k<m;k++) A[k]=A_tmp[k];
				}
			}
		}
		if (kt==1) N=m;
		else N=sizeof(A)/sizeof(float);
		average=0;
		for (k=0;k<N;k++){
			average=average+A[k];
		}
		average=average/N;
	}
	return average;
}

