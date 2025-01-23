function update()
    local dt = (clock:get_last_tick())

    self:move(self:get_pos("x") - math.floor(dt * 0.6), self:get_pos("y"))

    if (self:get_pos("x") < -self:get_size("x")) then
        game:remove_object(self)
        self = nil
    end
end
