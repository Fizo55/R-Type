function move_background()
    valuex = self:get_pos("x")
    if valuex < -2310 then
        valuex = valuex + 4608
    end
    valuex = valuex - 10
    self:move(valuex, 0)
end
