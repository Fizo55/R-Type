posx = self:get_pos("x")
posy = self:get_pos("y")
isLevel = false
function level1()

    if posx < 200 then
        posx = posx + 1
        self:move(posx, posy)
    end
    if posx == 200 then
        isLevel = true
        game:change_scene("level1")
        print("level 1 loaded")
        posx = 800
    end
end

