#定义伪目标
 
all:

	make -C ./../../psp_rel/usermodule/enhanced/make_build
	make -C ./../drv/welcome
	make -C ./../drv/lcd_ILI9341	
	make -C ./../drv/Key_216A_EVB
	make -C ./../drv/ui
	make -C ./../drv/fm_qn8035
	make -C ./../ap/common
	make -C ./../ap/ap_manager
	make -C ./../ap/ap_mainmenu		
	make -C ./../ap/ap_playlist		
	make -C ./../ap/ap_udisk		
	make -C ./../ap/ap_music
	make -C ./../ap/ap_config
	make -C ./../ap/music_engine
	make -C ./../ap/ap_setting	
	make -C ./../ap/ap_browser
	make -C ./../ap/fm_engine
	make -C ./../ap/ap_radio
	make -C ./../ap/ap_record	
	make -C ./../ap/ap_upgrade	


clean:
	make -C ./../../psp_rel/usermodule/enhanced/make_build clean
	make -C ./../drv/welcome clean
	make -C ./../drv/Key_216A_EVB clean
	make -C ./../drv/ui clean
	make -C ./../drv/fm_qn8035 clean	
	make -C ./../drv/lcd_ILI9341 clean
	make -C ./../ap/common clean
	make -C ./../ap/ap_manager clean
	make -C ./../ap/ap_mainmenu clean	
	make -C ./../ap/ap_playlist clean	
	make -C ./../ap/ap_udisk clean			
	make -C ./../ap/ap_music clean
	make -C ./../ap/ap_config clean
	make -C ./../ap/music_engine clean
	make -C ./../ap/ap_setting clean	
	make -C ./../ap/ap_browser clean	
	make -C ./../ap/fm_engine clean
	make -C ./../ap/ap_radio clean
	make -C ./../ap/ap_record clean	
	make -C ./../ap/ap_upgrade clean		

fw:

	make -C ./../../psp_rel/usermodule/enhanced/make_build
	make -C ./../drv/welcome
	make -C ./../drv/lcd_ILI9341	
	make -C ./../drv/Key_216A_EVB
	make -C ./../drv/ui
	make -C ./../drv/fm_qn8035
	make -C ./../ap/common
	make -C ./../ap/ap_manager
	make -C ./../ap/ap_mainmenu		
	make -C ./../ap/ap_playlist		
	make -C ./../ap/ap_udisk		
	make -C ./../ap/ap_music
	make -C ./../ap/ap_config
	make -C ./../ap/music_engine
	make -C ./../ap/ap_setting	
	make -C ./../ap/ap_browser
	make -C ./../ap/fm_engine
	make -C ./../ap/ap_radio
	make -C ./../ap/ap_record	
	make -C ./../ap/ap_upgrade
	cd "../fwpkg/"   && ./buildAFI_card.bat  && ./buildfw_card.bat