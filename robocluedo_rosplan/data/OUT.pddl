(define (problem task)
(:domain robocluedo)
(:objects
    center wp1 wp2 wp3 wp4 - waypoint
)
(:init

    (not-dirty)

    (is-center center)

    (not-is-center wp1)
    (not-is-center wp2)
    (not-is-center wp3)
    (not-is-center wp4)

    (at center)

    (passage center wp1)
    (passage wp1 center)
    (passage center wp4)
    (passage wp4 center)
    (passage wp1 wp2)
    (passage wp2 wp3)
    (passage wp3 wp4)

    (explored center)

    (not-explored wp1)
    (not-explored wp2)
    (not-explored wp3)
    (not-explored wp4)

    (at-center)



    (not-hint-ready)


    (not-hint-collected wp1)
    (not-hint-collected wp2)
    (not-hint-collected wp3)
    (not-hint-collected wp4)

)
(:goal (and
    (dirty)
))
)
