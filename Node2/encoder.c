

void encoder_init(){
	// TC_BMR_POSEN  enable position mode
	// TC_BMR_QDEN   enable quadrature decoder
	Tc.TC_BMR = TC_BMR_POSEN | TC_BMR_QDEN;
	
	// TC_CMR_ETRGEDG_RISING  detection on rising edge
	// TC_CMR_ABETRG		  Velg input A som trigger
	// TC_CMR_TCCLKS_XC0	  Velg klokke 0
	TcChannel.TC_CMR = TC_CMR_ETRGEDG_RISING | TC_CMR_ABETRG | TC_CMR_TCCLKS_XC0;
}


int32_t encoder_read(Encoder e){
	uint32_t registerPos = TcChannel.TC_CV;
	uint32_t registerDir = Tc.TC_QISR_DIR;
	
	e.pos = e.pos + registerPos * (2 * registerDir - 1);
	
	TcChannel.TC_CCR = 1; // reset counter
	return e.pos;
}


void encoder_reset(Encoder e){
	e.pos =  0;
}