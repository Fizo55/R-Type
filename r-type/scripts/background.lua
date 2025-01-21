function update()
    local valuex = self:get_pos("x")
    local dt = (clock:get_last_tick())

    if valuex < -self:get_size("x") then
        valuex = valuex + (self:get_size("x") * 2)
    end

    valuex = valuex - math.floor(dt * 0.3)

    self:move(valuex, self:get_pos("y"))
end
