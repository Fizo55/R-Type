function move_player()
    print("hello")
end


local angle = 10
local radius = 10
function update()
    local valuex = self:get_pos("x") + radius * math.cos(angle)
    local valuey = self:get_pos("y") + radius * math.sin(angle)
    
    self:move(valuex, valuey)

    angle = angle + 0.1
    print("update function")
end

