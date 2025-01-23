function are_colliding(obj, other)
    return (
        obj:get_pos("x") < other:get_pos("x") + other:get_size("x") and
        obj:get_pos("x") + obj:get_size("x") > other:get_pos("x") and
        obj:get_pos("y") < other:get_pos("y") + other:get_size("y") and
        obj:get_pos("y") + obj:get_size("y") > other:get_pos("y")
    )
end

function collision_bullet_to_ennemy(obj)
    local i = 0
    local other = game:get_object(i)

    while not (other == nil) do
        local name = other:get_name()

        if (name == "gen_ennemy") then
            if (are_colliding(obj, other)) then
                game:remove_object(obj)
                game:remove_object(other)
                game:write_int(game:read_int(0x00), game:read_int(game:read_int(0x00)) + 1)
                return
            end
        end

        i = i + 1
        other = game:get_object(i)
    end
end

function collision_player_to_ennemy(obj)
    local i = 0
    local other = game:get_object(i)

    while not (other == nil) do
        local name = other:get_name()

        if (name == "gen_ennemy" or name == "gen_ennemy_bullet") then
            if (are_colliding(obj, other)) then
                game:remove_object(obj)
                game:remove_object(other)
                return
            end
        end

        i = i + 1
        other = game:get_object(i)
    end
end

function update()
    local i = 0
    local temp = game:get_object(i)

    while not (temp == nil) do
        local name = temp:get_name()

        if (name == "gen_player_bullet") then
            collision_bullet_to_ennemy(temp)
        end

        if (name:find("^player") ~= nil) then
            collision_player_to_ennemy(temp)
        end

        i = i + 1
        temp = game:get_object(i)
    end
end