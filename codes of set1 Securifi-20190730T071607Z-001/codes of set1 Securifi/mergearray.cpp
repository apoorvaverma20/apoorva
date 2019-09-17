 #include<bits/stdc++.h>
 using namespace std;
 
 int merge(int arr1[],int arr2[],int n1,int n2)
 {
 	int arr3[100],n3,n4,k=0;
	n3=n1+n2;
	n4=abs(n2-n1);
	for(int i=0;i<n1;i++)
		arr3[i]=arr1[i];
	for(int j=n1;j<n3;j++)
	{
	    arr3[j]=arr2[k];
	    k++;
	}
	for(int i=0;i<n3;i++)
	cout<<arr3[i]<<"  ";
        cout<<endl;
}
 int main()
 {
 	int arr1[100],arr2[100];
 	int n1,n2;
 	cout<<"Enter the size of both the arrays\n";
 	cin>>n1>>n2;
 	cout<<"Enter the elements in array1\n";
 	for(int i=0;i<n1;i++)
 	cin>>arr1[i];
 	cout<<"Enter the elements in array2\n";
 	for(int i=0;i<n2;i++)
	cin>>arr2[i];
	
	merge(arr1,arr2,n1,n2);
 }
