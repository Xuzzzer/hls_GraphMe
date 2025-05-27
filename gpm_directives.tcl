# Pipeline critical loops in triangle_match
set_directive_pipeline "triangle_match/V1_LOOP"
set_directive_pipeline "triangle_match/V1_NEIGHBORS_LOOP"
set_directive_pipeline "triangle_match/INTERSECTION_LOOP"

# Pipeline get_neighbors (frequent use)
set_directive_pipeline "get_neighbors"

# Pipeline the set_intersection loop
set_directive_pipeline "your_set_intersection/COMPARE_LOOP"

# Improve memory access by partitioning local arrays in BRAM
set_directive_array_partition -type complete -dim 1 "neighbors_v1"
set_directive_array_partition -type complete -dim 1 "neighbors_v2"
set_directive_array_partition -type complete -dim 1 "intersection_result"
