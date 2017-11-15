unsigned long hash(unsigned char *str)
{
	unsigned int hash = 0;
	int c;
	
	while(c = *str++){
		hash += c;
	}
	
	return hash;
}