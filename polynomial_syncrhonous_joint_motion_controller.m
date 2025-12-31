function [configSoln, Q, t] = CartesianMover6(X, Y, Z, A, B, C, node, q0, T, dt)

    robot = importrobot("CPMOVER6.urdf");
    robot.DataFormat = "row";

    ik = inverseKinematics("RigidBodyTree", robot);
    weights = [0.25 0.25 0.25 1 1 1];
    initialGuess = homeConfiguration(robot);

    pub = ros2publisher(node, "/joint_demands", "sensor_msgs/JointState");

    A = deg2rad(A);
    B = deg2rad(B);
    C = deg2rad(C);

    Tx = [1 0 0 X;
          0 1 0 0;
          0 0 1 0;
          0 0 0 1];

    Ty = [1 0 0 0;
          0 1 0 Y;
          0 0 1 0;
          0 0 0 1];

    Tz = [1 0 0 0;
          0 1 0 0;
          0 0 1 Z;
          0 0 0 1];

    Rz = [ cos(A) -sin(A)  0  0;
           sin(A)  cos(A)  0  0;
           0       0       1  0;
           0       0       0  1];

    Ry = [ cos(B)  0  sin(B)  0;
           0       1  0       0;
          -sin(B)  0  cos(B)  0;
           0       0  0       1];

    Rx = [ 1  0        0       0;
           0  cos(C) -sin(C)  0;
           0  sin(C)  cos(C)  0;
           0  0        0       1];

    T_des = Tx * Ty * Tz * Rz * Ry * Rx;

    endEffectorName = "link6";
    [sol, ~] = ik(endEffectorName, T_des, weights, initialGuess);

    if isstruct(sol)
        configSoln = [sol.JointPosition];
    else
        configSoln = sol;
    end

    q1 = configSoln;

    N = floor(T / dt) + 1;
    t = linspace(0, T, N)';
    tau = t / T;

    s = 35 * tau.^4 ...
        - 84 * tau.^5 ...
        + 70 * tau.^6 ...
        - 20 * tau.^7;

    Q = q0 + s .* (q1 - q0);

    msg = ros2message(pub);
    for k = 1:size(Q, 1)
        msg.position = Q(k, :);
        send(pub, msg);
        pause(dt);
    end

end