

Declare
	@shift23_1 uniqueidentifier,
	@shift23_2 uniqueidentifier,
	@shift24_1 uniqueidentifier,
	@shift24_2 uniqueidentifier,
	@shift25_1 uniqueidentifier

Declare
	@preBiogas2Gen int,
	@preBiogas2Torch int,
	@preEnergy1 int,
	@preEnergy2 int,
	@preRuntime1 int,
	@preRuntime2 int,
	@biogas23_2_1 int,
	@biogas23_2_2 int,
	@biogas24_1_1 int,
	@biogas24_1_2 int,
	@biogas24_2_1 int,
	@biogas24_2_2 int,
	@biogas25_1_1 int,
	@biogas25_1_2 int,

	@energy23_2_1 int,
	@energy23_2_2 int,
	@energy24_1_1 int,
	@energy24_1_2 int,
	@energy24_2_1 int,
	@energy24_2_2 int,
	@energy25_1_1 int,
	@energy25_1_2 int

Select @biogas23_2_1 = 6100
Select @biogas23_2_2 = 12867 - @biogas23_2_1
Select @biogas24_1_1 = 6200
Select @biogas24_1_2 = 12630 - @biogas24_1_1
Select @biogas24_2_1 = 6500
Select @biogas24_2_2 = 13466 - @biogas24_2_1
Select @biogas25_1_1 = 6150
Select @biogas25_1_2 = 12936 - @biogas25_1_1

Select @energy23_2_1 = 13500
Select @energy23_2_2 = 26700 - @energy23_2_1
Select @energy24_1_1 = 13000
Select @energy24_1_2 = 26300 - @energy24_1_1
Select @energy24_2_1 = 14000
Select @energy24_2_2 = 27900 - @energy24_2_1
Select @energy25_1_1 = 13500
Select @energy25_1_2 = 26700 - @energy25_1_1


Select Top 1 @shift23_1 = ShiftId From ShiftStatMstr Where ActualBeginTime >= '2017-09-23 8:00:00' And ActualBeginTime <= '2017-09-23 20:00:00'
If (@@ROWCOUNT > 0)
	Begin
		Select @preBiogas2Gen = IsNull((Select SubTotalLast From ShiftStatDet Where ShiftId = @shift23_1 And Item = 'Biogas2GenSubtotal'), 0)
		Select @preBiogas2Torch = IsNull((Select SubTotalLast From ShiftStatDet Where ShiftId = @shift23_1 And Item = 'Biogas2TorchSubtotal'), 0)
		Select @preEnergy1 = IsNull((Select SubTotalLast From ShiftStatDet Where ShiftId = @shift23_1 And Item = 'EnergyProduction1'), 0)
		Select @preEnergy2 = IsNull((Select SubTotalLast From ShiftStatDet Where ShiftId = @shift23_1 And Item = 'EnergyProduction2'), 0)
		Select @preRuntime1 = IsNull((Select SubTotalLast From ShiftStatDet Where ShiftId = @shift23_1 And Item = 'SubtotalRuntime1'), 0)
		Select @preRuntime2 = IsNull((Select SubTotalLast From ShiftStatDet Where ShiftId = @shift23_1 And Item = 'SubtotalRuntime2'), 0)
	End
Else
	Begin			
		Select @preBiogas2Gen = 0
		Select @preBiogas2Torch = 0
		Select @preEnergy1 = 0
		Select @preEnergy2 = 0
		Select @preRuntime1 = 0
		Select @preRuntime2 = 0
	End

-- For 9.23
Select Top 1 @shift23_2 = ShiftId From ShiftStatMstr Where ActualBeginTime >= '2017-09-23 20:00:00' And ActualBeginTime <= '2017-09-24 8:00:00'
If (@@ROWCOUNT = 0)
	Begin
		Select @shift23_2 = NEWID()
		Insert Into ShiftStatMstr (ShiftId, BeginTime,ActualBeginTime, EndTime, LastUpdateTime,Status) Values (@shift23_2, '2017-09-23 20:00:00', '2017-09-23 20:00:00', '2017-09-24 8:00:00', '2017-09-24 8:00:00', 'A')
		Insert Into ShiftStatDet (ShiftId, Item, SubTotalBegin, SubTotalLast) Values(@shift23_2, 'Biogas2GenSubtotal', @preBiogas2Gen, @preBiogas2Gen + @biogas23_2_1)
		Insert Into ShiftStatDet (ShiftId, Item, SubTotalBegin, SubTotalLast) Values(@shift23_2, 'Biogas2TorchSubtotal', @preBiogas2Torch, @preBiogas2Torch + @biogas23_2_2)
		Insert Into ShiftStatDet (ShiftId, Item, SubTotalBegin, SubTotalLast) Values(@shift23_2, 'EnergyProduction1', @preEnergy1, @preEnergy1 + @energy23_2_1)
		Insert Into ShiftStatDet (ShiftId, Item, SubTotalBegin, SubTotalLast) Values(@shift23_2, 'EnergyProduction2', @preEnergy2, @preEnergy2 + @energy23_2_2)
		Insert Into ShiftStatDet (ShiftId, Item, SubTotalBegin, SubTotalLast) Values(@shift23_2, 'SubtotalRuntime1', @preRuntime1, @preRuntime1 + 12)
		Insert Into ShiftStatDet (ShiftId, Item, SubTotalBegin, SubTotalLast) Values(@shift23_2, 'SubtotalRuntime2', @preRuntime2, @preRuntime2 + 12)
	End
Else
	Begin
		Update ShiftStatMstr Set LastUpdateTime = '2017-09-24 8:00:00' Where ShiftId = @shift23_2
		Update ShiftStatDet Set SubTotalBegin = @preBiogas2Gen, SubTotalLast = @preBiogas2Gen + @biogas23_2_1 Where ShiftId = @shift23_2 And Item = 'Biogas2GenSubtotal'
		Update ShiftStatDet Set SubTotalBegin = @preBiogas2Torch, SubTotalLast = @preBiogas2Torch + @biogas23_2_2 Where ShiftId = @shift23_2 And Item = 'Biogas2TorchSubtotal'
		Update ShiftStatDet Set SubTotalBegin = @preEnergy1, SubTotalLast = @preEnergy1 + @energy23_2_1 Where ShiftId = @shift23_2 And Item = 'EnergyProduction1'
		Update ShiftStatDet Set SubTotalBegin = @preEnergy2, SubTotalLast = @preEnergy2 + @energy23_2_2 Where ShiftId = @shift23_2 And Item = 'EnergyProduction2'
		Update ShiftStatDet Set SubTotalBegin = @preRuntime1, SubTotalLast = @preRuntime1 + 12 Where ShiftId = @shift23_2 And Item = 'SubtotalRuntime1'
		Update ShiftStatDet Set SubTotalBegin = @preRuntime2, SubTotalLast = @preRuntime2 + 12 Where ShiftId = @shift23_2 And Item = 'SubtotalRuntime2'
	End

-- For 9.24 Shift #1
Select @preBiogas2Gen = IsNull((Select SubTotalLast From ShiftStatDet Where ShiftId = @shift23_2 And Item = 'Biogas2GenSubtotal'), 0)
Select @preBiogas2Torch = IsNull((Select SubTotalLast From ShiftStatDet Where ShiftId = @shift23_2 And Item = 'Biogas2TorchSubtotal'), 0)
Select @preEnergy1 = IsNull((Select SubTotalLast From ShiftStatDet Where ShiftId = @shift23_2 And Item = 'EnergyProduction1'), 0)
Select @preEnergy2 = IsNull((Select SubTotalLast From ShiftStatDet Where ShiftId = @shift23_2 And Item = 'EnergyProduction2'), 0)
Select @preRuntime1 = IsNull((Select SubTotalLast From ShiftStatDet Where ShiftId = @shift23_2 And Item = 'SubtotalRuntime1'), 0)
Select @preRuntime2 = IsNull((Select SubTotalLast From ShiftStatDet Where ShiftId = @shift23_2 And Item = 'SubtotalRuntime2'), 0)

Select Top 1 @shift24_1 = ShiftId From ShiftStatMstr Where ActualBeginTime >= '2017-09-24 8:00:00' And ActualBeginTime <= '2017-09-24 20:00:00'
If (@@ROWCOUNT = 0)
	Begin
		Select @shift24_1 = NEWID()
		Insert Into ShiftStatMstr (ShiftId, BeginTime,ActualBeginTime, EndTime, LastUpdateTime,Status) Values (@shift24_1, '2017-09-24 8:00:00', '2017-09-24 8:00:00', '2017-09-24 20:00:00', '2017-09-24 20:00:00', 'A')
		Insert Into ShiftStatDet (ShiftId, Item, SubTotalBegin, SubTotalLast) Values(@shift24_1, 'Biogas2GenSubtotal', @preBiogas2Gen, @preBiogas2Gen + @biogas24_1_1)
		Insert Into ShiftStatDet (ShiftId, Item, SubTotalBegin, SubTotalLast) Values(@shift24_1, 'Biogas2TorchSubtotal', @preBiogas2Torch, @preBiogas2Torch + @biogas24_1_2)
		Insert Into ShiftStatDet (ShiftId, Item, SubTotalBegin, SubTotalLast) Values(@shift24_1, 'EnergyProduction1', @preEnergy1, @preEnergy1 + @energy24_1_1)
		Insert Into ShiftStatDet (ShiftId, Item, SubTotalBegin, SubTotalLast) Values(@shift24_1, 'EnergyProduction2', @preEnergy2, @preEnergy2 + @energy24_1_2)
		Insert Into ShiftStatDet (ShiftId, Item, SubTotalBegin, SubTotalLast) Values(@shift24_1, 'SubtotalRuntime1', @preRuntime1, @preRuntime1 + 12)
		Insert Into ShiftStatDet (ShiftId, Item, SubTotalBegin, SubTotalLast) Values(@shift24_1, 'SubtotalRuntime2', @preRuntime2, @preRuntime2 + 12)
	End
Else
	Begin
		Update ShiftStatMstr Set LastUpdateTime = '2017-09-24 20:00:00' Where ShiftId = @shift24_1
		Update ShiftStatDet Set SubTotalBegin = @preBiogas2Gen, SubTotalLast = @preBiogas2Gen + @biogas24_1_1 Where ShiftId = @shift24_1 And Item = 'Biogas2GenSubtotal'
		Update ShiftStatDet Set SubTotalBegin = @preBiogas2Torch, SubTotalLast = @preBiogas2Torch + @biogas24_1_2 Where ShiftId = @shift24_1 And Item = 'Biogas2TorchSubtotal'
		Update ShiftStatDet Set SubTotalBegin = @preEnergy1, SubTotalLast = @preEnergy1 + @energy24_1_1 Where ShiftId = @shift24_1 And Item = 'EnergyProduction1'
		Update ShiftStatDet Set SubTotalBegin = @preEnergy2, SubTotalLast = @preEnergy2 + @energy24_1_2 Where ShiftId = @shift24_1 And Item = 'EnergyProduction2'
		Update ShiftStatDet Set SubTotalBegin = @preRuntime1, SubTotalLast = @preRuntime1 + 12 Where ShiftId = @shift24_1 And Item = 'SubtotalRuntime1'
		Update ShiftStatDet Set SubTotalBegin = @preRuntime2, SubTotalLast = @preRuntime2 + 12 Where ShiftId = @shift24_1 And Item = 'SubtotalRuntime2'
	End

-- For 9.24 Shift #2
Select @preBiogas2Gen = IsNull((Select SubTotalLast From ShiftStatDet Where ShiftId = @shift24_1 And Item = 'Biogas2GenSubtotal'), 0)
Select @preBiogas2Torch = IsNull((Select SubTotalLast From ShiftStatDet Where ShiftId = @shift24_1 And Item = 'Biogas2TorchSubtotal'), 0)
Select @preEnergy1 = IsNull((Select SubTotalLast From ShiftStatDet Where ShiftId = @shift24_1 And Item = 'EnergyProduction1'), 0)
Select @preEnergy2 = IsNull((Select SubTotalLast From ShiftStatDet Where ShiftId = @shift24_1 And Item = 'EnergyProduction2'), 0)
Select @preRuntime1 = IsNull((Select SubTotalLast From ShiftStatDet Where ShiftId = @shift24_1 And Item = 'SubtotalRuntime1'), 0)
Select @preRuntime2 = IsNull((Select SubTotalLast From ShiftStatDet Where ShiftId = @shift24_1 And Item = 'SubtotalRuntime2'), 0)

Select Top 1 @shift24_2 = ShiftId From ShiftStatMstr Where ActualBeginTime >= '2017-09-24 20:00:00' And ActualBeginTime <= '2017-09-25 8:00:00'
If (@@ROWCOUNT = 0)
	Begin
		Select @shift24_2 = NEWID()
		Insert Into ShiftStatMstr (ShiftId, BeginTime,ActualBeginTime, EndTime, LastUpdateTime,Status) Values (@shift24_2, '2017-09-24 20:00:00', '2017-09-24 20:00:00', '2017-09-25 8:00:00', '2017-09-25 8:00:00', 'A')
		Insert Into ShiftStatDet (ShiftId, Item, SubTotalBegin, SubTotalLast) Values(@shift24_2, 'Biogas2GenSubtotal', @preBiogas2Gen, @preBiogas2Gen + @biogas24_2_1)
		Insert Into ShiftStatDet (ShiftId, Item, SubTotalBegin, SubTotalLast) Values(@shift24_2, 'Biogas2TorchSubtotal', @preBiogas2Torch, @preBiogas2Torch + @biogas24_2_2)
		Insert Into ShiftStatDet (ShiftId, Item, SubTotalBegin, SubTotalLast) Values(@shift24_2, 'EnergyProduction1', @preEnergy1, @preEnergy1 + @energy24_2_1)
		Insert Into ShiftStatDet (ShiftId, Item, SubTotalBegin, SubTotalLast) Values(@shift24_2, 'EnergyProduction2', @preEnergy2, @preEnergy2 + @energy24_2_2)
		Insert Into ShiftStatDet (ShiftId, Item, SubTotalBegin, SubTotalLast) Values(@shift24_2, 'SubtotalRuntime1', @preRuntime1, @preRuntime1 + 12)
		Insert Into ShiftStatDet (ShiftId, Item, SubTotalBegin, SubTotalLast) Values(@shift24_2, 'SubtotalRuntime2', @preRuntime2, @preRuntime2 + 12)
	End
Else
	Begin
		Update ShiftStatMstr Set LastUpdateTime = '2017-09-25 8:00:00' Where ShiftId = @shift24_2
		Update ShiftStatDet Set SubTotalBegin = @preBiogas2Gen, SubTotalLast = @preBiogas2Gen + @biogas24_2_1 Where ShiftId = @shift24_2 And Item = 'Biogas2GenSubtotal'
		Update ShiftStatDet Set SubTotalBegin = @preBiogas2Torch, SubTotalLast = @preBiogas2Torch + @biogas24_2_2 Where ShiftId = @shift24_2 And Item = 'Biogas2TorchSubtotal'
		Update ShiftStatDet Set SubTotalBegin = @preEnergy1, SubTotalLast = @preEnergy1 + @energy24_2_1 Where ShiftId = @shift24_2 And Item = 'EnergyProduction1'
		Update ShiftStatDet Set SubTotalBegin = @preEnergy2, SubTotalLast = @preEnergy2 + @energy24_2_2 Where ShiftId = @shift24_2 And Item = 'EnergyProduction2'
		Update ShiftStatDet Set SubTotalBegin = @preRuntime1, SubTotalLast = @preRuntime1 + 12 Where ShiftId = @shift24_2 And Item = 'SubtotalRuntime1'
		Update ShiftStatDet Set SubTotalBegin = @preRuntime2, SubTotalLast = @preRuntime2 + 12 Where ShiftId = @shift24_2 And Item = 'SubtotalRuntime2'
	End

-- For 9.25 Shift #1
Select Top 1 @shift25_1 = ShiftId From ShiftStatMstr Where ActualBeginTime >= '2017-09-24 20:00:00' And ActualBeginTime <= '2017-09-25 8:00:00'
Update ShiftStatDet Set SubTotalBegin = SubTotalLast - @biogas25_1_1 Where ShiftId = @shift25_1 And Item = 'Biogas2GenSubtotal'
Update ShiftStatDet Set SubTotalBegin = SubTotalLast - @biogas25_1_2 Where ShiftId = @shift25_1 And Item = 'Biogas2TorchSubtotal'
Update ShiftStatDet Set SubTotalBegin = SubTotalLast - @energy25_1_1 Where ShiftId = @shift25_1 And Item = 'EnergyProduction1'
Update ShiftStatDet Set SubTotalBegin = SubTotalLast - @energy25_1_2 Where ShiftId = @shift25_1 And Item = 'EnergyProduction2'
Update ShiftStatDet Set SubTotalBegin = SubTotalLast - 12 Where ShiftId = @shift25_1 And Item = 'SubtotalRuntime1'
Update ShiftStatDet Set SubTotalBegin = SubTotalLast - 12 Where ShiftId = @shift25_1 And Item = 'SubtotalRuntime2'