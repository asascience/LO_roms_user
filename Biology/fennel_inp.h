      SUBROUTINE read_BioPar (model, inp, out, Lwrite)
!
!svn $Id: npzdo_Banas_inp.h 523 2011-01-05 03:21:38Z arango $
!================================================== Hernan G. Arango ===
!  Copyright (c) 2002-2011 The ROMS/TOMS Group                         !
!    Licensed under a MIT/X style license                              !
!    See License_ROMS.txt                                              !
!=======================================================================
!                                                                      !
!  This routine reads in npzd_Banas ecosystem model input    !
!  parameters. They are specified in input script "npzdo_Banas.in".         !
!                                                                      !
!=======================================================================
!
      USE mod_param
      USE mod_parallel
      USE mod_biology
      USE mod_ncparam
      USE mod_scalars
!
      implicit none
!
!  Imported variable declarations
!
      logical, intent(in) :: Lwrite
      integer, intent(in) :: model, inp, out
!
!  Local variable declarations.
!
      integer :: Npts, Nval
      integer :: iTrcStr, iTrcEnd
      integer :: i, ifield, igrid, itracer, itrc, ng, nline, status

      integer :: decode_line, load_i, load_l, load_lbc, load_r

      logical, dimension(Ngrids) :: Lbio
      logical, dimension(NBT,Ngrids) :: Ltrc

      real(r8), dimension(NBT,Ngrids) :: Rbio

      real(r8), dimension(100) :: Rval

      character (len=40 ) :: KeyWord
      character (len=256) :: line
      character (len=256), dimension(200) :: Cval
!
!-----------------------------------------------------------------------
!  Initialize.
!-----------------------------------------------------------------------
!
      igrid=1                            ! nested grid counter
      itracer=0                          ! LBC tracer counter
      iTrcStr=1                          ! first LBC tracer to process
      iTrcEnd=NBT                        ! last  LBC tracer to process
      nline=0                            ! LBC multi-line counter
!
!-----------------------------------------------------------------------
!  Read in Banas NPZD2O biological model parameters.
!-----------------------------------------------------------------------
!
      DO WHILE (.TRUE.)
        READ (inp,'(a)',ERR=10,END=20) line
        status=decode_line(line, KeyWord, Nval, Cval, Rval)
        IF (status.gt.0) THEN
          SELECT CASE (TRIM(KeyWord))
            CASE ('Lbiology') 
              Npts=load_l(Nval, Cval, Ngrids, Lbiology)
            CASE ('BioIter') 
              Npts=load_i(Nval, Rval, Ngrids, BioIter)
            
            CASE ('PARfrac')
              Npts=load_r(Nval, Rval, Ngrids, PARfrac)
            CASE ('AttSW')
              Npts=load_r(Nval, Rval, Ngrids, AttSW)
            CASE ('AttP')
              Npts=load_r(Nval, Rval, Ngrids, AttP)
            CASE ('AttS')
              Npts=load_r(Nval, Rval, Ngrids, AttS)
            CASE ('phyAlpha')
              Npts=load_r(Nval, Rval, Ngrids, phyAlpha)
            CASE ('phyMu0')
              Npts=load_r(Nval, Rval, Ngrids, phyMu0)
            CASE ('phyKs')
              Npts=load_r(Nval, Rval, Ngrids, phyKs)
            CASE ('phyMin')
              Npts=load_r(Nval, Rval, Ngrids, phyMin)
            CASE ('phyM')
              Npts=load_r(Nval, Rval, Ngrids, phyM)
            
            CASE ('zooI0')
              Npts=load_r(Nval, Rval, Ngrids, zooI0)
            CASE ('zooKs')
              Npts=load_r(Nval, Rval, Ngrids, zooKs)
            CASE ('zooEps')
              Npts=load_r(Nval, Rval, Ngrids, zooEps)
            CASE ('zooFegest')
              Npts=load_r(Nval, Rval, Ngrids, zooFegest)
            CASE ('zooMin')
              Npts=load_r(Nval, Rval, Ngrids, zooMin)
            CASE ('zooZeta')
              Npts=load_r(Nval, Rval, Ngrids, zooZeta)
            
            CASE ('detRemin')
              Npts=load_r(Nval, Rval, Ngrids, detRemin)
            CASE ('CoagR')
              Npts=load_r(Nval, Rval, Ngrids, CoagR)
            CASE ('detWsink')
              Npts=load_r(Nval, Rval, Ngrids, detWsink)
            CASE ('LdetWsink')
              Npts=load_r(Nval, Rval, Ngrids, LdetWsink)
	    CASE ('pCO2air')
              Npts=load_r(Nval, Rval, Ngrids, pCO2air)
            
            CASE ('TNU2')
              Npts=load_r(Nval, Rval, NBT*Ngrids, Rbio)
            DO ng=1,Ngrids
              DO itrc=1,NBT
                i=idbio(itrc)
                nl_tnu2(i,ng)=Rbio(itrc,ng)
              END DO
            END DO
            CASE ('TNU4')
              Npts=load_r(Nval, Rval, NBT*Ngrids, Rbio)
            DO ng=1,Ngrids
              DO itrc=1,NBT
                i=idbio(itrc)
                nl_tnu4(i,ng)=Rbio(itrc,ng)
              END DO
            END DO
	    CASE ('LtracerSponge')
	      Npts=load_l(Nval, Cval, NBT*Ngrids, Ltrc)
	    DO ng=1,Ngrids
	      DO itrc=1,NBT
	        i=idbio(itrc)
   	        LtracerSponge(i,ng)=Ltrc(itrc,ng)
              END DO
            END DO
            CASE ('AKT_BAK')
              Npts=load_r(Nval, Rval, NBT*Ngrids, Rbio)
            DO ng=1,Ngrids
              DO itrc=1,NBT
                i=idbio(itrc)
                Akt_bak(i,ng)=Rbio(itrc,ng)
              END DO
            END DO
            CASE ('ad_AKT_fac')
              Npts=load_r(Nval, Rval, NBT*Ngrids, Rbio)
              DO ng=1,Ngrids
                DO itrc=1,NBT
                  i=idbio(itrc)
                  ad_Akt_fac(i,ng)=Rbio(itrc,ng)
                  tl_Akt_fac(i,ng)=Rbio(itrc,ng)
                END DO
              END DO
            CASE ('TNUDG')
              Npts=load_r(Nval, Rval, NBT*Ngrids, Rbio)
              DO ng=1,Ngrids
                DO itrc=1,NBT
                  i=idbio(itrc)
                  Tnudg(i,ng)=Rbio(itrc,ng)
                END DO
              END DO
            CASE ('LBC(isTvar)')
              IF (itracer.lt.NBT) THEN
                itracer=itracer+1
              ELSE
                itracer=1                      ! next nested grid
              END IF
              ifield=isTvar(idbio(itracer))
              Npts=load_lbc(Nval, Cval, line, nline, ifield, igrid,     &
     &                      idbio(iTrcStr), idbio(iTrcEnd),             &     
     &                      Vname(1,idTvar(idbio(itracer))), LBC)
#if defined ADJOINT || defined TANGENT || defined TL_IOMS
            CASE ('ad_LBC(isTvar)')
              IF (itracer.lt.NBT) THEN
                itracer=itracer+1
              ELSE
                itracer=1                      ! next nested grid
              END IF
              ifield=isTvar(idbio(itracer))
              Npts=load_lbc(Nval, Cval, line, nline, ifield, igrid,     &
     &                      idbio(iTrcStr), idbio(iTrcEnd),             &
     &                      Vname(1,idTvar(idbio(itracer))), ad_LBC)
#endif

            CASE ('LtracerCLM')
              Npts=load_l(Nval, Cval, NBT*Ngrids, Ltrc)
              DO ng=1,Ngrids
                DO itrc=1,NBT
                  i=idbio(itrc)
                  LtracerCLM(i,ng)=Ltrc(itrc,ng)
                END DO
              END DO
	    CASE ('LnudgeTCLM')
              Npts=load_l(Nval, Cval, NBT*Ngrids, Ltrc)
              DO ng=1,Ngrids
                DO itrc=1,NBT
                  i=idbio(itrc)
                  LnudgeTCLM(i,ng)=Ltrc(itrc,ng)
                END DO
              END DO
            CASE ('LtracerSrc')
              Npts=load_l(Nval, Cval, NBT*Ngrids, Ltrc)
              DO ng=1,Ngrids
                DO itrc=1,NBT
                  i=idbio(itrc)
                  LtracerSrc(i,ng)=Ltrc(itrc,ng)
                END DO
              END DO
            CASE ('Hout(idTvar)')
              Npts=load_l(Nval, Cval, NBT*Ngrids, Ltrc)
              DO ng=1,Ngrids
                DO itrc=1,NBT
                  i=idTvar(idbio(itrc))
                  IF (i.eq.0) THEN
                    IF (Master) WRITE (out,30)                          &
     &                                'idTvar(idbio(', itrc, '))'
                    exit_flag=5
                    RETURN
                  END IF
                  Hout(i,ng)=Ltrc(itrc,ng)
                END DO
              END DO
            CASE ('Hout(idTsur)')
              Npts=load_l(Nval, Cval, NBT*Ngrids, Ltrc)
              DO ng=1,Ngrids
                DO itrc=1,NBT
                  i=idTsur(idbio(itrc))
                  IF (i.eq.0) THEN
                    IF (Master) WRITE (out,30)                          &
     &                                'idTsur(idbio(', itrc, '))'
                    exit_flag=5
                    RETURN
                  END IF
                  Hout(i,ng)=Ltrc(itrc,ng)
                END DO
              END DO
	   CASE ('Qout(idTvar)')
	     Npts=load_l(Nval, Cval, NBT*Ngrids, Ltrc)
	     DO ng=1,Ngrids
	       DO itrc=1,NBT
	         i=idTvar(idbio(itrc))
	         Qout(i,ng)=Ltrc(itrc,ng)
               END DO
             END DO
	  CASE ('Qout(idsurT)')
	    Npts=load_l(Nval, Cval, NBT*Ngrids, Ltrc)
	    DO ng=1,Ngrids
	      DO itrc=1,NBT
	        i=idsurT(idbio(itrc))
	        IF (i.eq.0) THEN
	          IF (Master) WRITE (out,30)                          &
     &                                'idsurT(idbio(', itrc, '))'
                  exit_flag=5
                  RETURN
                END IF
	        Qout(i,ng)=Ltrc(itrc,ng)
              END DO
            END DO
	  CASE ('Qout(idTsur)')
	    Npts=load_l(Nval, Cval, NBT*Ngrids, Ltrc)
	    DO ng=1,Ngrids
	      DO itrc=1,NBT
	        i=idTsur(idbio(itrc))
	        Qout(i,ng)=Ltrc(itrc,ng)
              END DO
            END DO

#if defined AVERAGES    || \
   (defined AD_AVERAGES && defined ADJOINT) || \
   (defined RP_AVERAGES && defined TL_IOMS) || \
   (defined TL_AVERAGES && defined TANGENT)
            CASE ('Aout(idTvar)')
              Npts=load_l(Nval, Cval, NBT*Ngrids, Ltrc)
              DO ng=1,Ngrids
                DO itrc=1,NBT
                  i=idTvar(idbio(itrc))
                  Aout(i,ng)=Ltrc(itrc,ng)
                END DO
              END DO
            CASE ('Aout(idTTav)')
              Npts=load_l(Nval, Cval, NBT*Ngrids, Ltrc)
              DO ng=1,Ngrids
                DO itrc=1,NBT
                  i=idTTav(idbio(itrc))
                  Aout(i,ng)=Ltrc(itrc,ng)
                END DO
              END DO
            CASE ('Aout(idUTav)')
              Npts=load_l(Nval, Cval, NBT*Ngrids, Ltrc)
              DO ng=1,Ngrids
                DO itrc=1,NBT
                  i=idUTav(idbio(itrc))
                  Aout(i,ng)=Ltrc(itrc,ng)
                END DO
              END DO
            CASE ('Aout(idVTav)')
              Npts=load_l(Nval, Cval, NBT*Ngrids, Ltrc)
              DO ng=1,Ngrids
                DO itrc=1,NBT
                  i=idVTav(idbio(itrc))
                  Aout(i,ng)=Ltrc(itrc,ng)
                END DO
              END DO
            CASE ('Aout(iHUTav)')
              Npts=load_l(Nval, Cval, NBT*Ngrids, Ltrc)
              DO ng=1,Ngrids
                DO itrc=1,NBT
                  i=iHUTav(idbio(itrc))
                  Aout(i,ng)=Ltrc(itrc,ng)
                END DO
              END DO
            CASE ('Aout(iHVTav)')
              Npts=load_l(Nval, Cval, NBT*Ngrids, Ltrc)
              DO ng=1,Ngrids
                DO itrc=1,NBT
                  i=iHVTav(idbio(itrc))
                  Aout(i,ng)=Ltrc(itrc,ng)
                END DO
              END DO
#endif
#ifdef DIAGNOSTICS_TS
            CASE ('Dout(iTrate)')
              Npts=load_l(Nval, Cval, NBT*Ngrids, Ltrc)
              DO ng=1,Ngrids
                DO i=1,NBT
                  itrc=idbio(i)
                  Dout(idDtrc(itrc,iTrate),ng)=Ltrc(i,ng)
                END DO
              END DO
            CASE ('Dout(iThadv)')
              Npts=load_l(Nval, Cval, NBT*Ngrids, Ltrc)
              DO ng=1,Ngrids
                DO i=1,NBT
                  itrc=idbio(i)
                  Dout(idDtrc(itrc,iThadv),ng)=Ltrc(i,ng)
                END DO
              END DO
            CASE ('Dout(iTxadv)')
              Npts=load_l(Nval, Cval, NBT*Ngrids, Ltrc)
              DO ng=1,Ngrids
                DO i=1,NBT
                  itrc=idbio(i)
                  Dout(idDtrc(itrc,iTxadv),ng)=Ltrc(i,ng)
                END DO
              END DO
            CASE ('Dout(iTyadv)')
              Npts=load_l(Nval, Cval, NBT*Ngrids, Ltrc)
              DO ng=1,Ngrids
                DO i=1,NBT
                  itrc=idbio(i)
                  Dout(idDtrc(itrc,iTyadv),ng)=Ltrc(i,ng)
                END DO
              END DO
            CASE ('Dout(iTvadv)')
              Npts=load_l(Nval, Cval, NBT*Ngrids, Ltrc)
              DO ng=1,Ngrids
                DO i=1,NBT
                  itrc=idbio(i)
                  Dout(idDtrc(itrc,iTvadv),ng)=Ltrc(i,ng)
                END DO
              END DO
# if defined TS_DIF2 || defined TS_DIF4
            CASE ('Dout(iThdif)')
              Npts=load_l(Nval, Cval, NBT*Ngrids, Ltrc)
              DO ng=1,Ngrids
                DO i=1,NBT
                  itrc=idbio(i)
                  Dout(idDtrc(itrc,iThdif),ng)=Ltrc(i,ng)
                END DO
              END DO
            CASE ('Dout(iTxdif)')
              Npts=load_l(Nval, Cval, NBT*Ngrids, Ltrc)
              DO ng=1,Ngrids
                DO i=1,NBT
                  itrc=idbio(i)
                  Dout(idDtrc(itrc,iTxdif),ng)=Ltrc(i,ng)
                END DO
              END DO
            CASE ('Dout(iTydif)')
              Npts=load_l(Nval, Cval, NBT*Ngrids, Ltrc)
              DO ng=1,Ngrids
                DO i=1,NBT
                  itrc=idbio(i)
                  Dout(idDtrc(itrc,iTydif),ng)=Ltrc(i,ng)
                END DO
              END DO
#  if defined MIX_GEO_TS || defined MIX_ISO_TS
            CASE ('Dout(iTsdif)')
              Npts=load_l(Nval, Cval, NBT*Ngrids, Ltrc)
              DO ng=1,Ngrids
                DO i=1,NBT
                  itrc=idbio(i)
                  Dout(idDtrc(itrc,iTsdif),ng)=Ltrc(i,ng)
                END DO
              END DO
#  endif
# endif
            CASE ('Dout(iTvdif)')
              Npts=load_l(Nval, Cval, NBT*Ngrids, Ltrc)
              DO ng=1,Ngrids
                DO i=1,NBT
                  itrc=idbio(i)
                  Dout(idDtrc(itrc,iTvdif),ng)=Ltrc(i,ng)
                END DO
              END DO
#endif
#  ifdef DIAGNOSTICS_BIO
#   ifdef OXYGEN
            CASE('Dout(iO2fx)')
				IF (iDbio2(iO2fx).eq.0) THEN
				  IF (Master) WRITE (out,40) 'iDbio2(iO2fx)'
				  exit_flag=5
				  RETURN
				END IF
				  Npts=load_l(Nval, Cval, Ngrids, Lbio)
				i=iDbio2(iO2fx)
				DO ng=1,Ngrids
				  Dout(i,ng)=Lbio(ng)
				END DO
			CASE('Dout(iDsed)')
				IF (iDbio2(iDsed).eq.0) THEN
				  IF (Master) WRITE (out,40) 'iDbio2(iDsed)'
				  exit_flag=5
				  RETURN
				END IF
				  Npts=load_l(Nval, Cval, Ngrids, Lbio)
				i=iDbio2(iDsed)
				DO ng=1,Ngrids
				  Dout(i,ng)=Lbio(ng)
				END DO
			CASE('Dout(iDNIT)')
				IF (iDbio3(iDNIT).eq.0) THEN
				  IF (Master) WRITE (out,40) 'iDbio3(iDNIT)'
				  exit_flag=5
				  RETURN
				END IF
				  Npts=load_l(Nval, Cval, Ngrids, Lbio)
				i=iDbio3(iDNIT)
				DO ng=1,Ngrids
				  Dout(i,ng)=Lbio(ng)
				END DO
#   endif
# ifdef CARBON
            CASE ('Dout(iCOfx)')
              IF (iDbio2(iCOfx).eq.0) THEN
                IF (Master) WRITE (out,40) 'iDbio2(iCOfx)'
                exit_flag=5
                RETURN
              END IF
              Npts=load_l(Nval, Cval, Ngrids, Lbio)
              i=iDbio2(iCOfx)
              DO ng=1,Ngrids
                Dout(i,ng)=Lbio(ng)
              END DO
# endif
# ifdef CARBON
            CASE ('Dout(ipCO2)')
              IF (iDbio2(ipCO2).eq.0) THEN
                IF (Master) WRITE (out,40) 'iDbio2(ipCO2)'
                exit_flag=5
                RETURN
              END IF
              Npts=load_l(Nval, Cval, Ngrids, Lbio)
              i=iDbio2(ipCO2)
              DO ng=1,Ngrids
                Dout(i,ng)=Lbio(ng)
              END DO
# endif
            CASE('Dout(iPPro)')
				IF (iDbio3(iPPro).eq.0) THEN
				  IF (Master) WRITE (out,40) 'iDbio3(iPPro)'
				  exit_flag=5
				  RETURN
				END IF
				  Npts=load_l(Nval, Cval, Ngrids, Lbio)
				i=iDbio3(iPPro)
				DO ng=1,Ngrids
				  Dout(i,ng)=Lbio(ng)
				END DO
            CASE('Dout(iGraz)')
				IF (iDbio3(iGraz).eq.0) THEN
				  IF (Master) WRITE (out,40) 'iDbio3(iGraz)'
				  exit_flag=5
				  RETURN
				END IF
				  Npts=load_l(Nval, Cval, Ngrids, Lbio)
				i=iDbio3(iGraz)
				DO ng=1,Ngrids
				  Dout(i,ng)=Lbio(ng)
				END DO
            CASE('Dout(iRem)')
				IF (iDbio3(iRem).eq.0) THEN
				  IF (Master) WRITE (out,40) 'iDbio3(iRem)'
				  exit_flag=5
				  RETURN
				END IF
				  Npts=load_l(Nval, Cval, Ngrids, Lbio)
				i=iDbio3(iRem)
				DO ng=1,Ngrids
				  Dout(i,ng)=Lbio(ng)
				END DO
#  endif
          END SELECT
        END IF
      END DO
  10  IF (Master) WRITE (out,50) line
      exit_flag=4
      RETURN
  20  CONTINUE
!
!-----------------------------------------------------------------------
!  Report input parameters.
!-----------------------------------------------------------------------
!
      IF (Lwrite) THEN
        DO ng=1,Ngrids
          IF (Lbiology(ng)) THEN
            WRITE (out,40) ng
            WRITE (out,70) BioIter(ng), 'BioIter',                      &
     &            'Number of iterations for nonlinear convergence.'
            WRITE (out,80) AttSW(ng), 'AttSW',                         &
     &            'Light attenuation of seawater (m-1).'
            WRITE (out,80) AttP(ng), 'AttP',                           &
     &            'Light attenuation by chlorophyll (1/(mg_Chl m-2)).'
! insert tedious code to report other parameters here
#ifdef TS_DIF2
            DO itrc=1,NBT
              i=idbio(itrc)
              WRITE (out,100) nl_tnu2(i,ng), 'nl_tnu2', i,              &
     &              'NLM Horizontal, harmonic mixing coefficient',      &
     &              '(m2/s) for tracer ', i, TRIM(Vname(1,idTvar(i)))
# ifdef ADJOINT
              WRITE (out,100) ad_tnu2(i,ng), 'ad_tnu2', i,              &
     &              'ADM Horizontal, harmonic mixing coefficient',      &
     &              '(m2/s) for tracer ', i, TRIM(Vname(1,idTvar(i)))
# endif
# if defined TANGENT || defined TL_IOMS
              WRITE (out,100) tl_tnu2(i,ng), 'tl_tnu2', i,              &
     &              'TLM Horizontal, harmonic mixing coefficient',      &
     &              '(m2/s) for tracer ', i, TRIM(Vname(1,idTvar(i)))
# endif
            END DO
#endif
#ifdef TS_DIF4
            DO itrc=1,NBT
              i=idbio(itrc)
              WRITE (out,100) nl_tnu4(i,ng), 'nl_tnu4', i,              &
     &              'NLM Horizontal, biharmonic mixing coefficient',    &
     &              '(m4/s) for tracer ', i, TRIM(Vname(1,idTvar(i)))
# ifdef ADJOINT
              WRITE (out,100) ad_tnu4(i,ng), 'ad_tnu4', i,              &
     &              'ADM Horizontal, biharmonic mixing coefficient',    &
     &              '(m4/s) for tracer ', i, TRIM(Vname(1,idTvar(i)))
# endif
# if defined TANGENT || defined TL_IOMS
              WRITE (out,100) tl_tnu4(i,ng), 'tl_tnu4', i,              &
     &              'TLM Horizontal, biharmonic mixing coefficient',    &
     &              '(m4/s) for tracer ', i, TRIM(Vname(1,idTvar(i)))
# endif
            END DO
#endif
            DO itrc=1,NBT
              i=idbio(itrc)
              IF (LtracerSponge(i,ng)) THEN
                WRITE (out,110) LtracerSponge(i,ng), 'LtracerSponge',   &
     &              i, 'Turning ON  sponge on tracer ', i,              &
     &              TRIM(Vname(1,idTvar(i)))
              ELSE
                WRITE (out,110) LtracerSponge(i,ng), 'LtracerSponge',   &
     &              i, 'Turning OFF sponge on tracer ', i,              &
     &              TRIM(Vname(1,idTvar(i)))
              END IF
            END DO
            DO itrc=1,NBT
              i=idbio(itrc)
              WRITE(out,100) Akt_bak(i,ng), 'Akt_bak', i,               &
     &             'Background vertical mixing coefficient (m2/s)',     &
     &             'for tracer ', i, TRIM(Vname(1,idTvar(i)))
            END DO
#ifdef FORWARD_MIXING
            DO itrc=1,NBT
              i=idbio(itrc)
# ifdef ADJOINT
              WRITE (out,100) ad_Akt_fac(i,ng), 'ad_Akt_fac', i,        &
     &              'ADM basic state vertical mixing scale factor',     &
     &              'for tracer ', i, TRIM(Vname(1,idTvar(i)))
# endif
# if defined TANGENT || defined TL_IOMS
              WRITE (out,100) tl_Akt_fac(i,ng), 'tl_Akt_fac', i,        &
     &              'TLM basic state vertical mixing scale factor',     &
     &              'for tracer ', i, TRIM(Vname(1,idTvar(i)))
# endif
            END DO
#endif
            DO itrc=1,NBT
              i=idbio(itrc)
              WRITE (out,100) Tnudg(i,ng), 'Tnudg', i,                  &
     &              'Nudging/relaxation time scale (days)',             &
     &              'for tracer ', i, TRIM(Vname(1,idTvar(i)))
            END DO
            DO itrc=1,NBT
              i=idbio(itrc)
              IF (LtracerSrc(i,ng)) THEN
                WRITE (out,110) LtracerSrc(i,ng), 'LtracerSrc',         &
     &              i, 'Turning ON  point sources/Sink on tracer ', i,  &
     &              TRIM(Vname(1,idTvar(i)))
              ELSE
                WRITE (out,110) LtracerSrc(i,ng), 'LtracerSrc',         &
     &              i, 'Turning OFF point sources/Sink on tracer ', i,  &
     &              TRIM(Vname(1,idTvar(i)))
              END IF
            END DO
            DO itrc=1,NBT
              i=idbio(itrc)
              IF (LtracerCLM(i,ng)) THEN
                WRITE (out,110) LtracerCLM(i,ng), 'LtracerCLM', i,      &
     &              'Turning ON  processing of climatology tracer ', i, &
     &              TRIM(Vname(1,idTvar(i)))
              ELSE
                WRITE (out,110) LtracerCLM(i,ng), 'LtracerCLM', i,      &
     &              'Turning OFF processing of climatology tracer ', i, &
     &              TRIM(Vname(1,idTvar(i)))
              END IF
            END DO
            DO itrc=1,NBT
              i=idbio(itrc)
              IF (LnudgeTCLM(i,ng)) THEN
                WRITE (out,110) LnudgeTCLM(i,ng), 'LnudgeTCLM', i,      &
     &              'Turning ON  nudging of climatology tracer ', i,    &
     &              TRIM(Vname(1,idTvar(i)))
              ELSE
                WRITE (out,110) LnudgeTCLM(i,ng), 'LnudgeTCLM', i,      &
     &              'Turning OFF nudging of climatology tracer ', i,    &
     &              TRIM(Vname(1,idTvar(i)))
              END IF
            END DO
            IF ((nHIS(ng).gt.0).and.ANY(Hout(:,ng))) THEN
              WRITE (out,'(1x)')
              DO itrc=1,NBT
                i=idbio(itrc)
                IF (Hout(idTvar(i),ng)) WRITE (out,120)                 &
     &              Hout(idTvar(i),ng), 'Hout(idTvar)',                 &
     &              'Write out tracer ', i, TRIM(Vname(1,idTvar(i)))
              END DO
              DO itrc=1,NBT
                i=idbio(itrc)
                IF (Hout(idTsur(i),ng)) WRITE (out,120)                 &
     &              Hout(idTsur(i),ng), 'Hout(idTsur)',                 &
     &              'Write out tracer flux ', i,                        &
     &              TRIM(Vname(1,idTvar(i)))
              END DO
            END IF
            IF ((nQCK(ng).gt.0).and.ANY(Qout(:,ng))) THEN
              WRITE (out,'(1x)')
              DO itrc=1,NBT
                i=idbio(itrc)
                IF (Qout(idTvar(i),ng)) WRITE (out,120)                 &
     &              Qout(idTvar(i),ng), 'Qout(idTvar)',                 &
     &              'Write out tracer ', i, TRIM(Vname(1,idTvar(i)))
              END DO
              DO itrc=1,NBT
                i=idbio(itrc)
                IF (Qout(idsurT(i),ng)) WRITE (out,120)                 &
     &              Qout(idsurT(i),ng), 'Qout(idsurT)',                 &
     &              'Write out surface tracer ', i,                     &
     &              TRIM(Vname(1,idTvar(i)))
              END DO
              DO itrc=1,NBT
                i=idbio(itrc)
                IF (Qout(idTsur(i),ng)) WRITE (out,120)                 &
     &              Qout(idTsur(i),ng), 'Qout(idTsur)',                 &
     &              'Write out tracer flux ', i,                        &
     &              TRIM(Vname(1,idTvar(i)))
              END DO
            END IF
#if defined AVERAGES    || \
   (defined AD_AVERAGES && defined ADJOINT) || \
   (defined RP_AVERAGES && defined TL_IOMS) || \
   (defined TL_AVERAGES && defined TANGENT)
            IF ((nAVG(ng).gt.0).and.ANY(Aout(:,ng))) THEN
              WRITE (out,'(1x)')
              DO itrc=1,NBT
                i=idbio(itrc)
                IF (Aout(idTvar(i),ng)) WRITE (out,120)                 &
     &              Aout(idTvar(i),ng), 'Aout(idTvar)',                 &
     &              'Write out averaged tracer ', i,                    &
     &              TRIM(Vname(1,idTvar(i)))
              END DO
              DO itrc=1,NBT
                i=idbio(itrc)
                IF (Aout(idTTav(i),ng)) WRITE (out,120)                 &
     &              Aout(idTTav(i),ng), 'Aout(idTTav)',                 &
     &              'Write out averaged <t*t> for tracer ', i,          &
     &              TRIM(Vname(1,idTvar(i)))
              END DO
              DO itrc=1,NBT
                i=idbio(itrc)
                IF (Aout(idUTav(i),ng)) WRITE (out,120)                 &
     &              Aout(idUTav(i),ng), 'Aout(idUTav)',                 &
     &              'Write out averaged <u*t> for tracer ', i,          &
     &              TRIM(Vname(1,idTvar(i)))
              END DO
              DO itrc=1,NBT
                i=idbio(itrc)
                IF (Aout(idVTav(i),ng)) WRITE (out,120)                 &
     &              Aout(idVTav(i),ng), 'Aout(idVTav)',                 &
     &              'Write out averaged <v*t> for tracer ', i,          &
     &              TRIM(Vname(1,idTvar(i)))
              END DO
              DO itrc=1,NBT
                i=idbio(itrc)
                IF (Aout(iHUTav(i),ng)) WRITE (out,120)                 &
     &              Aout(iHUTav(i),ng), 'Aout(iHUTav)',                 &
     &              'Write out averaged <Huon*t> for tracer ', i,       &
     &              TRIM(Vname(1,idTvar(i)))
              END DO
              DO itrc=1,NBT
                i=idbio(itrc)
                IF (Aout(iHVTav(i),ng)) WRITE (out,120)                 &
     &              Aout(iHVTav(i),ng), 'Aout(iHVTav)',                 &
     &              'Write out averaged <Hvom*t> for tracer ', i,       &
     &              TRIM(Vname(1,idTvar(i)))
              END DO
            END IF
#endif
#ifdef DIAGNOSTICS_TS
            IF ((nDIA(ng).gt.0).and.ANY(Dout(:,ng))) THEN
              WRITE (out,'(1x)')
              DO i=1,NBT
                itrc=idbio(i)
                IF (Dout(idDtrc(itrc,iTrate),ng))                         &
     &            WRITE (out,120) .TRUE., 'Dout(iTrate)',                 &
     &                'Write out rate of change of tracer ', itrc,        &
     &                TRIM(Vname(1,idTvar(itrc)))
              END DO
              DO i=1,NBT
                itrc=idbio(i)
                IF (Dout(idDtrc(itrc,iThadv),ng))                         &
     &            WRITE (out,120) .TRUE., 'Dout(iThadv)',                 &
     &                'Write out horizontal advection, tracer ', itrc,    &
     &                TRIM(Vname(1,idTvar(itrc)))
              END DO
              DO i=1,NBT
                itrc=idbio(i)
                IF (Dout(idDtrc(itrc,iTxadv),ng))                         &
     &            WRITE (out,120) .TRUE., 'Dout(iTxadv)',                 &
     &                'Write out horizontal X-advection, tracer ', itrc,  &
     &                TRIM(Vname(1,idTvar(itrc)))
              END DO
              DO i=1,NBT
                itrc=idbio(i)
                IF (Dout(idDtrc(itrc,iTyadv),ng))                         &
     &            WRITE (out,120) .TRUE., 'Dout(iTyadv)',                 &
     &                'Write out horizontal Y-advection, tracer ', itrc,  &
     &                TRIM(Vname(1,idTvar(itrc)))
              END DO
              DO i=1,NBT
                itrc=idbio(i)
                IF (Dout(idDtrc(itrc,iTvadv),ng))                         &
     &            WRITE (out,120) .TRUE., 'Dout(iTvadv)',                 &
     &                'Write out vertical advection, tracer ', itrc,      &
     &                TRIM(Vname(1,idTvar(itrc)))
              END DO
# if defined TS_DIF2 || defined TS_DIF4
              DO i=1,NBT
                itrc=idbio(i)
                IF (Dout(idDtrc(itrc,iThdif),ng))                         &
     &            WRITE (out,120) .TRUE., 'Dout(iThdif)',                 &
     &                'Write out horizontal diffusion, tracer ', itrc,    &
     &                TRIM(Vname(1,idTvar(itrc)))
              END DO
              DO i=1,NBT
                itrc=idbio(i)
                IF (Dout(idDtrc(i,iTxdif),ng))                            &
     &            WRITE (out,120) .TRUE., 'Dout(iTxdif)',                 &
     &                'Write out horizontal X-diffusion, tracer ', itrc,  &
     &                TRIM(Vname(1,idTvar(itrc)))
              END DO
              DO i=1,NBT
                itrc=idbio(i)
                IF (Dout(idDtrc(itrc,iTydif),ng))                         &
     &            WRITE (out,120) .TRUE., 'Dout(iTydif)',                 &
     &                'Write out horizontal Y-diffusion, tracer ', itrc,  &
     &                TRIM(Vname(1,idTvar(itrc)))
              END DO
#  if defined MIX_GEO_TS || defined MIX_ISO_TS
              DO i=1,NBT
                itrc=idbio(i)
                IF (Dout(idDtrc(itrc,iTsdif),ng))                         &
     &            WRITE (out,120) .TRUE., 'Dout(iTsdif)',                 &
     &                'Write out horizontal S-diffusion, tracer ', itrc,  &
     &                TRIM(Vname(1,idTvar(itrc)))
              END DO
#  endif
# endif
              DO i=1,NBT
                itrc=idbio(i)
                IF (Dout(idDtrc(itrc,iTvdif),ng))                         &
     &            WRITE (out,120) .TRUE., 'Dout(iTvdif)',                 &
     &                'Write out vertical diffusion, tracer ', itrc,      &
     &                TRIM(Vname(1,idTvar(itrc)))
              END DO
            END IF
#endif

#  ifdef DIAGNOSTICS_BIO
       IF (nDIA(ng).gt.0) THEN
#   if !(defined CARBON || defined OXYGEN || defined DENITRIFICATION) 
            DO itrc=1,NDbio2d
              i=iDbio2(itrc)
              IF (Dout(i,ng)) WRITE (out,140)                           &
     &            Dout(i,ng), 'Hout(iDbio2)', 'Diagnostics for',        &
     &            TRIM(Vname(1,i))
            END DO
#   endif
            DO itrc=1,NDbio3d
              i=iDbio3(itrc)
              IF (Dout(i,ng)) WRITE (out,140)                           &
     &            Dout(i,ng), 'Hout(iDbio3)', 'Diagnostics for',        &
     &            TRIM(Vname(1,i))
            END DO
	END IF
#  endif
          END IF
        END DO
      END IF
!
!-----------------------------------------------------------------------
!  Rescale biological tracer parameters
!-----------------------------------------------------------------------
!
!  Take the square root of the biharmonic coefficients so it can
!  be applied to each harmonic operator.
!
      DO ng=1,Ngrids
        DO itrc=1,NBT
          i=idbio(itrc)
          nl_tnu4(i,ng)=SQRT(ABS(nl_tnu4(i,ng)))
!
!  Compute inverse nudging coefficients (1/s) used in various tasks.
!
          IF (Tnudg(i,ng).gt.0.0_r8) THEN
            Tnudg(i,ng)=1.0_r8/(Tnudg(i,ng)*86400.0_r8)
          ELSE
            Tnudg(i,ng)=0.0_r8
          END IF
        END DO
      END DO

  30  FORMAT (/,' read_BioPar - variable info not yet loaded, ',        &
     &        a,i2.2,a)
!!30  FORMAT (/,' read_BioPar - Error while processing line: ',/,a)
  40  FORMAT (/,/,' Banas NPZDO Model Parameters, Grid: ',i2.2,              &
     &        /,  ' =================================',/)  
  70  FORMAT (1x,i10,2x,a,t32,a)
  80  FORMAT (1p,e11.4,2x,a,t32,a)
  90  FORMAT (1p,e11.4,2x,a,t32,a,/,t34,a)
  100  FORMAT (1p,e11.4,2x,a,'(',i2.2,')',t32,a,/,t34,a,i2.2,':',1x,a)
  110  FORMAT (10x,l1,2x,a,'(',i2.2,')',t32,a,i2.2,':',1x,a)
  120  FORMAT (10x,l1,2x,a,t32,a,i2.2,':',1x,a)
  130  FORMAT (10x,l1,2x,a,t32,a,1x,a)

! 70  FORMAT (1x,i10,2x,a,t32,a)
!  80  FORMAT (1p,e11.4,2x,a,t32,a)
!  90  FORMAT (1p,e11.4,2x,a,t32,a,/,t34,a)
! 100  FORMAT (1p,e11.4,2x,a,'(',i2.2,')',t32,a,/,t34,a,i2.2,':',1x,a)
! 110  FORMAT (10x,l1,2x,a,'(',i2.2,')',t32,a,i2.2,':',1x,a)
! 120  FORMAT (10x,l1,2x,a,t32,a,i2.2,':',1x,a)
! 130  FORMAT (10x,l1,2x,a,t32,a,1x,a)

  50  FORMAT (1x,i10,2x,a,t30,a)
!  60  FORMAT (10x,l1,2x,a,t32,a,i2.2,':',1x,a)
!  70  FORMAT (f11.3,2x,a,t30,a)
!  80  FORMAT (f11.3,2x,a,t30,a,/,t32,a)
!  90  FORMAT (1p,e11.4,2x,a,'(',i2.2,')',t30,a,/,t32,a,i2.2,':',1x,a)
! 100  FORMAT (1p,e11.4,2x,a,t30,a)
! 110  FORMAT (1p,e11.4,2x,a,t30,a,/,t32,a)
! 120  FORMAT (/,' read_BioPar - variable info not yet loaded, ',       &
!     &        a,i2.2,a)
! 130  FORMAT (/,' read_BioPar - variable info not yet loaded, ',a)
 140  FORMAT (10x,l1,2x,a,t30,a,1x,a)
 150  FORMAT (10x,l1,2x,a,'(',i2.2,')',t30,a,i2.2,':',1x,a)

      RETURN
      END SUBROUTINE read_BioPar