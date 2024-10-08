#include "homa_base.h"


extern int main();
extern void SystemInit();
extern void SystemCoreClockUpdate();

#ifdef __cplusplus
extern "C" {
#endif

extern void __libc_init_array();
extern void __libc_fini_array();
void Reset_Handler(void);
void Default_Handler(void);

#ifdef __cplusplus
}
#endif
/* Function definitions */

[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void HardFault_Handler(void);
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void MemManage_Handler(void);
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void BusFault_Handler(void);
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void UsageFault_Handler(void);
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void SVCall_Handler(void);
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void DebugMonitor_Handler(void);
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void PendSV_Handler(void);
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void Systick_Handler(void);
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void WWDG_Handler(void);                  
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void PVD_Handler(void);                  
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void TAMP_STAMP_Handler(void);            
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void RTC_WKUP_Handler(void);              
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void FLASH_Handler(void);                
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void RCC_Handler(void);                   
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void EXTI0_Handler(void);                 
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void EXTI1_Handler(void);                 
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void EXTI2_Handler(void);                 
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void EXTI3_Handler(void);                 
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void EXTI4_Handler(void);                 
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void DMA1_Stream0_Handler(void);          
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void DMA1_Stream1_Handler(void);          
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void DMA1_Stream2_Handler(void);          
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void DMA1_Stream3_Handler(void);          
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void DMA1_Stream4_Handler(void);          
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void DMA1_Stream5_Handler(void);          
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void DMA1_Stream6_Handler(void);          
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void ADC_Handler(void);                   
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void CAN1_TX_Handler(void);               
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void CAN1_RX0_Handler(void);              
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void CAN1_RX1_Handler(void);              
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void CAN1_SCE_Handler(void);              
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void EXTI9_5_Handler(void);              
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void TIM1_BRK_TIM9_Handler(void);         
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void TIM1_UP_TIM10_Handler(void);         
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void TIM1_TRG_COM_TIM11_Handler(void);    
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void TIM1_CC_Handler(void);               
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void TIM2_Handler(void);                 
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void TIM3_Handler(void);                  
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void TIM4_Handler(void);                  
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void I2C1_EV_Handler(void);               
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void I2C1_ER_Handler(void);               
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void I2C2_EV_Handler(void);               
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void I2C2_ER_Handler(void);               
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void SPI1_Handler(void);                  
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void SPI2_Handler(void);                  
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void USART1_Handler(void);                
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void USART2_Handler(void);                
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void USART3_Handler(void);                
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void EXTI15_10_Handler(void);             
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void RTC_Alarm_Handler(void);             
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void OTG_FS_WKUP_Handler(void);           
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void TIM8_BRK_TIM12_Handler(void);       
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void TIM8_UP_TIM13_Handler(void);         
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void TIM8_TRG_COM_TIM14_Handler(void);    
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void TIM8_CC_Handler(void);               
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void DMA1_Stream7_Handler(void);          
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void FSMC_Handler(void);                  
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void SDIO_Handler(void);                  
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void TIM5_Handler(void);                  
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void SPI3_Handler(void);                  
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void UART4_Handler(void);                 
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void UART5_Handler(void);                 
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void TIM6_DAC_Handler(void);             
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void TIM7_Handler(void);                 
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void DMA2_Stream0_Handler(void);          
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void DMA2_Stream1_Handler(void);          
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void DMA2_Stream2_Handler(void);          
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void DMA2_Stream3_Handler(void);          
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void DMA2_Stream4_Handler(void);          
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void ETH_Handler(void);                  
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void ETH_WKUP_Handler(void);             
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void CAN2_TX_Handler(void);               
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void CAN2_RX0_Handler(void);              
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void CAN2_RX1_Handler(void);              
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void CAN2_SCE_Handler(void);              
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void OTG_FS_Handler(void);                
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void DMA2_Stream5_Handler(void);          
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void DMA2_Stream6_Handler(void);          
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void DMA2_Stream7_Handler(void);          
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void USART6_Handler(void);               
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void I2C3_EV_Handler(void);              
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void I2C3_ER_Handler(void);               
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void OTG_HS_EP1_OUT_Handler(void);        
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void OTG_HS_EP1_IN_Handler(void);         
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void OTG_HS_WKUP_Handler(void);           
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void OTG_HS_Handler(void);                
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void DCMI_Handler(void);                  
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void CRYP_Handler(void);                  
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void HASH_RNG_Handler(void);              
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void FPU_Handler(void);                   
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void UART7_Handler(void);                 
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void UART8_Handler(void);                 
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void SPI4_Handler(void);                  
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void SPI5_Handler(void);                  
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void SPI6_Handler(void);                  
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void SAI1_Handler(void);                  
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void LCDTFT_Handler(void);                
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void LCDTFT_ERR_Handler(void);            
[[gnu::weak, gnu::alias("Default_Handler"), gnu::nothrow]] void DMA2D_Handler(void);                 

extern std::uint32_t _estack;
extern std::uint32_t _etext;
extern std::uint32_t _sdata;
extern std::uint32_t _sidata;
extern std::uint32_t _edata;
extern std::uint32_t _sccram;
extern std::uint32_t _eccram;
extern std::uint32_t _sbss;
extern std::uint32_t _ebss;


std::uint32_t vectors[] __attribute__((section(".isr_vector"))) = {
  STACK_START,
  (std::uint32_t)&Reset_Handler,
  (std::uint32_t)&HardFault_Handler,
  (std::uint32_t)&MemManage_Handler, 
  (std::uint32_t)&BusFault_Handler,
  (std::uint32_t)&UsageFault_Handler,       
  0, 
  0, 
  0, 
  0,
  (std::uint32_t)&SVCall_Handler,             
  (std::uint32_t)&DebugMonitor_Handler,        
  0,
  (std::uint32_t)&PendSV_Handler,             
  (std::uint32_t)&Systick_Handler,           
  (std::uint32_t)&WWDG_Handler,          
  (std::uint32_t)&PVD_Handler,                  
  (std::uint32_t)&TAMP_STAMP_Handler,         
  (std::uint32_t)&RTC_WKUP_Handler,           
  (std::uint32_t)&FLASH_Handler,            
  (std::uint32_t)&RCC_Handler,                  
  (std::uint32_t)&EXTI0_Handler,              
  (std::uint32_t)&EXTI1_Handler,            
  (std::uint32_t)&EXTI2_Handler,              
  (std::uint32_t)&EXTI3_Handler,             
  (std::uint32_t)&EXTI4_Handler,               
  (std::uint32_t)&DMA1_Stream0_Handler,        
  (std::uint32_t)&DMA1_Stream1_Handler,      
  (std::uint32_t)&DMA1_Stream2_Handler,        
  (std::uint32_t)&DMA1_Stream3_Handler,        
  (std::uint32_t)&DMA1_Stream4_Handler,        
  (std::uint32_t)&DMA1_Stream5_Handler,      
  (std::uint32_t)&DMA1_Stream6_Handler,       
  (std::uint32_t)&ADC_Handler,                
  (std::uint32_t)&CAN1_TX_Handler,             
  (std::uint32_t)&CAN1_RX0_Handler,         
  (std::uint32_t)&CAN1_RX1_Handler,        
  (std::uint32_t)&CAN1_SCE_Handler,            
  (std::uint32_t)&EXTI9_5_Handler,        
  (std::uint32_t)&TIM1_BRK_TIM9_Handler,       
  (std::uint32_t)&TIM1_UP_TIM10_Handler,     
  (std::uint32_t)&TIM1_TRG_COM_TIM11_Handler,   
  (std::uint32_t)&TIM1_CC_Handler,            
  (std::uint32_t)&TIM2_Handler,                
  (std::uint32_t)&TIM3_Handler,             
  (std::uint32_t)&TIM4_Handler,             
  (std::uint32_t)&I2C1_EV_Handler,           
  (std::uint32_t)&I2C1_ER_Handler,         
  (std::uint32_t)&I2C2_EV_Handler,           
  (std::uint32_t)&I2C2_ER_Handler,       
  (std::uint32_t)&SPI1_Handler,              
  (std::uint32_t)&SPI2_Handler,        
  (std::uint32_t)&USART1_Handler,            
  (std::uint32_t)&USART2_Handler,      
  (std::uint32_t)&USART3_Handler,           
  (std::uint32_t)&EXTI15_10_Handler,            
  (std::uint32_t)&RTC_Alarm_Handler,   
  (std::uint32_t)&OTG_FS_WKUP_Handler,      
  (std::uint32_t)&TIM8_BRK_TIM12_Handler,      
  (std::uint32_t)&TIM8_UP_TIM13_Handler,       
  (std::uint32_t)&TIM8_TRG_COM_TIM14_Handler, 
  (std::uint32_t)&TIM8_CC_Handler,             
  (std::uint32_t)&DMA1_Stream7_Handler,       
  (std::uint32_t)&FSMC_Handler,                 
  (std::uint32_t)&SDIO_Handler,               
  (std::uint32_t)&TIM5_Handler,           
  (std::uint32_t)&SPI3_Handler,              
  (std::uint32_t)&UART4_Handler,              
  (std::uint32_t)&UART5_Handler,            
  (std::uint32_t)&TIM6_DAC_Handler,         
  (std::uint32_t)&TIM7_Handler,            
  (std::uint32_t)&DMA2_Stream0_Handler,  
  (std::uint32_t)&DMA2_Stream1_Handler,       
  (std::uint32_t)&DMA2_Stream2_Handler,  
  (std::uint32_t)&DMA2_Stream3_Handler,    
  (std::uint32_t)&DMA2_Stream4_Handler,   
  (std::uint32_t)&ETH_Handler,              
  (std::uint32_t)&ETH_WKUP_Handler,      
  (std::uint32_t)&CAN2_TX_Handler,        
  (std::uint32_t)&CAN2_RX0_Handler,           
  (std::uint32_t)&CAN2_RX1_Handler,         
  (std::uint32_t)&CAN2_SCE_Handler,           
  (std::uint32_t)&OTG_FS_Handler,            
  (std::uint32_t)& DMA2_Stream5_Handler,      
  (std::uint32_t)&DMA2_Stream6_Handler,     
  (std::uint32_t)&DMA2_Stream7_Handler,     
  (std::uint32_t)&USART6_Handler,         
  (std::uint32_t)&I2C3_EV_Handler,          
  (std::uint32_t)&I2C3_ER_Handler,          
  (std::uint32_t)&OTG_HS_EP1_OUT_Handler,     
  (std::uint32_t)&OTG_HS_EP1_IN_Handler,    
  (std::uint32_t)&OTG_HS_WKUP_Handler,      
  (std::uint32_t)&OTG_HS_Handler,            
  (std::uint32_t)&DCMI_Handler,             
  (std::uint32_t)&CRYP_Handler,           
  (std::uint32_t)&HASH_RNG_Handler,       
  (std::uint32_t)&FPU_Handler,              
  (std::uint32_t)&UART7_Handler,            
  (std::uint32_t)&UART8_Handler,           
  (std::uint32_t)&SPI4_Handler,               
  (std::uint32_t)&SPI5_Handler,             
  (std::uint32_t)&SPI6_Handler,         
  (std::uint32_t)&SAI1_Handler,      
  (std::uint32_t)&LCDTFT_Handler,           
  (std::uint32_t)&LCDTFT_ERR_Handler,     
  (std::uint32_t)&DMA2D_Handler           
};

void Reset_Handler(void){

  /* Copy .data section from ROM (FLASH) to RAM, copy one byte at a time */
  std::uint32_t size = (std::uint32_t)&_edata - (std::uint32_t)&_sdata;
  std::uint8_t *dst = (std::uint8_t*)&_sdata; // RAM
  std::uint8_t *src = (std::uint8_t*)&_sidata; // FLASH
  for(std::uint32_t i = 0; i < size; i++){
    *dst++ = *src++;
  }

  /* Init the .bss section to 0 */
  size = (std::uint32_t)&_ebss - (std::uint32_t)&_sbss;
  dst = (std::uint8_t*)&_sbss;
  for(std::uint32_t i = 0; i < size; i++){
    *dst++ = 0;
  }

  /* Init stuff for standard lib */
  __libc_init_array();

  SystemInit();
  /* Call main */
  main();

  /* Fini stuff for standard lib */
  __libc_fini_array();

}

void Default_Handler(void){
  while(1);
}

