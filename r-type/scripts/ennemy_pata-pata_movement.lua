iterator = 0

posx = self:get_pos("x")
posy = self:get_pos("y")

while iterator < 500 do
    posx = posx - 1
    posy = posy + 1
    self:move(posx, posy)
end

function update()
end