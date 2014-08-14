SELECT id
FROM Offer
WHERE accountId=lower('%q') AND retargetingId like '%%%q%%'
;
