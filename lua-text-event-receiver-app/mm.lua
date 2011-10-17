function redraw (text)
	print(text)
	canvas:attrColor('black')
	canvas:drawRect('fill', 0,0, canvas:attrSize())
	canvas:attrColor('white')
	canvas:attrFont('vera', 28)
	canvas:drawText(10,10, text)
	canvas:flush()
end


function handler (evt)
	print('Dentro de Lua: Recebi evento')
	
	if evt.class == 'multimodal' then
		print('Dentro de Lua: Multimodal')
		print(evt.stringCount)
		print(evt.string1)
		if (evt.stringCount >= '1') then
			print('Dentro de Lua: evt.stringCount >= 1')
			redraw(evt.string1)
		end
	else
		print('Dentro de Lua: Não-multimodal')
		
		if evt.class == 'key' then
			print('Dentro de Lua: key')
			
			if evt.type == 'press' then
				print('Dentro de Lua: press')
				
				if evt.key >= '1' and evt.key <= '9' then
					print('Dentro de Lua: Número')
					
					event.post {
						class  = 'ncl',
						type   = 'presentation',
						action = 'stop',
					}
				end
			end
		end
	end
end

event.register(handler)
