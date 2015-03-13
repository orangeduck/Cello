
local pi = 3.141592653589793
local solar_mass = 4 * pi * pi
local days_per_year = 365.24

local jupiter = {
    x=4.84143144246472090e+00,
    y=-1.16032004402742839e+00,
    z=-1.03622044471123109e-01,
    vx=days_per_year * 1.66007664274403694e-03,
    vy=days_per_year * 7.69901118419740425e-03,
    vz=days_per_year * -6.90460016972063023e-05,
    mass=solar_mass * 9.54791938424326609e-04}

local saturn = {
  x=8.34336671824457987e+00,
  y=4.12479856412430479e+00,
  z=-4.03523417114321381e-01,
  vx=days_per_year * -2.76742510726862411e-03,
  vy=days_per_year * 4.99852801234917238e-03,
  vz=days_per_year * 2.30417297573763929e-05,
  mass=solar_mass * 2.85885980666130812e-04}

local uranus = {
  x=1.28943695621391310e+01,
  y=-1.51111514016986312e+01,
  z=-2.23307578892655734e-01,
  vx=days_per_year * 2.96460137564761618e-03,
  vy=days_per_year * 2.37847173959480950e-03,
  vz=days_per_year * -2.96589568540237556e-05,
  mass=solar_mass * 4.36624404335156298e-05}

local neptune = {
  x=1.53796971148509165e+01,
  y=-2.59193146099879641e+01,
  z=1.79258772950371181e-01,
  vx=days_per_year * 2.68067772490389322e-03,
  vy=days_per_year * 1.62824170038242295e-03,
  vz=days_per_year * -9.51592254519715870e-05,
  mass=solar_mass * 5.15138902046611451e-05}

local sun = {
   x=0.0,  y=0.0,  z=0.0,
  vx=0.0, vy=0.0, vz=0.0,
  mass=solar_mass}

function body_offset_momentum (b, px, py, pz)
    b.vx = -px / solar_mass
    b.vy = -py / solar_mass
    b.vz = -pz / solar_mass
end

function bodies_advance(bodies, dt)

    for i = 1, #bodies do

        local body0 = bodies[i]

        for j = i+1, #bodies do

            local body1 = bodies[j];

            local dx = body0.x - body1.x;
            local dy = body0.y - body1.y;
            local dz = body0.z - body1.z;

            local dsquared = dx * dx + dy * dy + dz * dz;
            local distance = math.sqrt(dsquared);
            local mag = dt / (dsquared * distance);

            body0.vx = body0.vx - dx * body1.mass * mag;
            body0.vy = body0.vy -dy * body1.mass * mag;
            body0.vz = body0.vz -dz * body1.mass * mag;

            body1.vx = body1.vx + dx * body0.mass * mag;
            body1.vy = body1.vy + dy * body0.mass * mag;
            body1.vz = body1.vz + dz * body0.mass * mag;
        end
    end

    for i = 1, #bodies do
        body = bodies[i];
        body.x = body.x + dt * body.vx;
        body.y = body.y + dt * body.vy;
        body.z = body.z + dt * body.vz;
    end
        
end
        
function bodies_energy(bodies)

    local dx = 0.0;
    local dy = 0.0;
    local dz = 0.0;
    local distance = 0.0;
    local e = 0.0;

    for i = 1, #bodies.length do

        local body0 = bodies[i];

        e = e + (0.5 * body0.mass * (
            body0.vx * body0.vx +
            body0.vy * body0.vy +
            body0.vz * body0.vz));

        for j = i+1, #bodies do

            local body1 = bodies[j];

            dx = body0.x - body1.x;
            dy = body0.y - body1.y;
            dz = body0.z - body1.z;

            distance = sqrt(dx * dx + dy * dy + dz * dz);
            e = e - (body0.mass * body1.mass) / distance;
        end

    end

    return e;
end



local bodies = {jupiter, saturn, uranus, neptune, sun};

local px = 0.0;
local py = 0.0;
local pz = 0.0;

for i = 1, #bodies do
    local body = bodies[i];
    px = px + body.vx * body.mass;
    py = py + body.vy * body.mass;
    pz = pz + body.vz * body.mass;
end

body_offset_momentum(bodies[1], px, py, pz);

for i = 1, 100000 do
    bodies_advance(bodies, 1e-5)
end



